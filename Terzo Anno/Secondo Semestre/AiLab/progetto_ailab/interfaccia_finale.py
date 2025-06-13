
"""
Questo è il codice principale per l'interfaccia Gradio di Formula AI.
L'interfaccia consente di visualizzare i dati di gara e fare previsioni sui risultati delle gare di Formula 1.
è presente una home e una pagina per la predizione.
"""

from __future__ import annotations

import warnings
from pathlib import Path
from dataclasses import dataclass
from typing import List, Optional, Any, Dict, Tuple

import gradio as gr
import numpy as np
import pandas as pd
from joblib import load
from sklearn.pipeline import Pipeline

warnings.filterwarnings("ignore", category=UserWarning, module="sklearn.utils.validation")
warnings.filterwarnings("ignore", category=FutureWarning)

# ── helper --------------------------------------------------------------------
try:
    from utils_gradio import to_str_type  # Nostra funzione per i dati
    print("✅  util to_str_type imported from utils_gradio.py")
except ImportError:
    print("⚠️  utils_gradio.py not found – defining local to_str_type fallback.")

    def to_str_type(X_input):  # type: ignore
        if isinstance(X_input, pd.Series):
            return X_input.astype(str)
        if isinstance(X_input, pd.DataFrame):
            X_out = X_input.copy()
            for col in X_out.columns:
                X_out[col] = X_out[col].astype(str)
            return X_out
        if isinstance(X_input, np.ndarray):
            return X_input.astype(str)
        print(f"[to_str_type] unexpected type: {type(X_input)} – returning as‑is")
        return X_input

# ── configuration -------------------------------------------------------------
BASE_DIR = Path(__file__).resolve().parent
DATA_DIR = BASE_DIR / "data"
MODELS_DIR = BASE_DIR / "models"
VIDEO_DIR = BASE_DIR / "video" # Roba vecchia

YEAR_CURR = 2025  # Stagione corrente ( non finita)
GP_YEARS = [2023, 2024, YEAR_CURR]

# datasets & models -----------------------------------------------------------
HISTORICAL_M1_PATH = DATA_DIR / "training_dataset_COMPLETO.csv"  # 2023‑24
M1_2025_PATH = DATA_DIR / f"training_dataset_{YEAR_CURR}.csv"  # gare 2025 disputate fino ad ora
CALENDAR_PATH = DATA_DIR / f"gare_{YEAR_CURR}.csv"  # Tutto il calendario 2025
PRE_RACE_DATASET_M2 = DATA_DIR / "pre_race_prediction_dataset.csv"  # dataset pre-gara

MODEL1_LGBM_PATH = MODELS_DIR / "lgbm_pipeline.joblib"
MODEL1_CB_PATH = MODELS_DIR / "catboost_pipeline.joblib"
MODEL2_CB_PATH = MODELS_DIR / "pre_race_catboost_pipeline.joblib"




# feature sets ----------------------------------------------------------------
NUM_COLS_M1 = [
    "lap_time",
    "lap_time_diff",
    "lap_time_pct",
    "gap_to_leader_diff",
    "gap_to_leader_pct",
    "speed_avg",
    "throttle_pct",
    "brake_pct",
    "drs_pct",
    "gear_avg",
    "sector1",
    "sector2",
    "sector3",
    "stint",
    "lap_in_stint",
    "elo_driver",
    "elo_team",
    "position",
    "lap_number",
]
CAT_COLS_M1 = ["team", "driver", "compound", "pit_in", "pit_out", "track_status"]
FEATURES_M1 = NUM_COLS_M1 + CAT_COLS_M1
TARGET_M1 = "final_position"

YEARS_LOOKBACK_M2 = 2
NUM_COLS_M2 = [
    "starting_grid_position",
    "elo_driver_pre_race",
    "elo_team_pre_race",
    f"avg_finish_pos_circuit_prev{YEARS_LOOKBACK_M2}y",
    f"avg_grid_pos_circuit_prev{YEARS_LOOKBACK_M2}y",
    f"avg_pos_gained_circuit_prev{YEARS_LOOKBACK_M2}y",
    f"races_on_circuit_prev{YEARS_LOOKBACK_M2}y",
    f"best_finish_pos_circuit_prev{YEARS_LOOKBACK_M2}y",
    f"laps_completed_pct_circuit_prev{YEARS_LOOKBACK_M2}y",
    f"team_avg_finish_pos_circuit_prev{YEARS_LOOKBACK_M2}y",
    f"team_avg_pos_gained_circuit_prev{YEARS_LOOKBACK_M2}y",
]
CAT_COLS_M2 = ["driver", "team", "gp", "is_driver_new_to_circuit"]
FEATURES_M2 = NUM_COLS_M2 + CAT_COLS_M2

# table display mappings ------------------------------------------------------
SITUATION_DISPLAY = {
    "driver": "Pilota",
    "team": "Team",
    "position_display": "Pos.",
    "lap_time": "T. Giro",
    "gap_leader_display": "Gap Leader",
    "compound": "Gomma"
}
PRED_M1_DISPLAY = {
    "driver": "Pilota",
    "team": "Team",
    "predicted_position": "Pos. Predetta (Mod.1)",
    "actual_position": "Pos. Reale",
    "difference": "Diff.",
}
PRED_M2_DISPLAY = {
    "driver": "Pilota",
    "team": "Team",
    "predicted_position": "Pos. Predetta (Mod.2)",
}

# dataclass ---------------------------------------------
@dataclass
class LapDataM1:
    X: Optional[pd.DataFrame] = None
    y_true: Optional[pd.Series] = None
    display_raw: Optional[pd.DataFrame] = None
    error: Optional[str] = None

# globals loaded at start -----------------------------------------------------
model1_lgbm: Optional[Pipeline] = None
model1_cb: Optional[Pipeline] = None
model2_cb: Optional[Pipeline] = None

ALL_M1_DF: pd.DataFrame = pd.DataFrame()
PRE_RACE_DF: pd.DataFrame = pd.DataFrame()
CALENDAR_DF: pd.DataFrame = pd.DataFrame()

DISPUTED_RACES: set[tuple[int, str]] = set()
ELO_CACHE: Dict[str, Dict[str, float]] = {}

# ── resource loading ---------------------------------------------------------

# Questa funzione viene chiamata all'avvio dell'app per caricare i modelli e i dataset
def load_resources() -> None:
    """Load models + datasets – run once at startup."""
    global model1_lgbm, model1_cb, model2_cb
    global ALL_M1_DF, PRE_RACE_DF, CALENDAR_DF, DISPUTED_RACES, ELO_CACHE

    # models ------------------------------------------------------------------
    if MODEL1_LGBM_PATH.exists():
        model1_lgbm = load(MODEL1_LGBM_PATH)
        print("✅  Model‑1 LGBM loaded")
    if MODEL1_CB_PATH.exists():
        model1_cb = load(MODEL1_CB_PATH)
        print("✅  Model‑1 CatBoost loaded")
    if MODEL2_CB_PATH.exists():
        model2_cb = load(MODEL2_CB_PATH)
        print("✅  Model‑2 CatBoost loaded")

    # datasets ----------------------------------------------------------------
    df_parts: list[pd.DataFrame] = []
    if HISTORICAL_M1_PATH.exists():
        df_parts.append(pd.read_csv(HISTORICAL_M1_PATH))
    if M1_2025_PATH.exists():
        df_parts.append(pd.read_csv(M1_2025_PATH))
    if df_parts:
        ALL_M1_DF = pd.concat(df_parts, ignore_index=True)
        # pulizia i dati
        for col in ["gp", "driver", "team"] + CAT_COLS_M1:
            if col in ALL_M1_DF.columns:
                ALL_M1_DF[col] = ALL_M1_DF[col].astype(str)
        if "anno" in ALL_M1_DF.columns:
            ALL_M1_DF["anno"] = pd.to_numeric(ALL_M1_DF["anno"], errors="coerce").astype("Int64")
        # build cache of disputed (anno, gp)
        DISPUTED_RACES = set(
            tuple(x) for x in ALL_M1_DF[["anno", "gp"]].dropna().drop_duplicates().values
        )
        print(f"✅  M1 combined DF loaded – {len(ALL_M1_DF)} rows, {len(DISPUTED_RACES)} GPs")
        
    if PRE_RACE_DATASET_M2.exists():
        PRE_RACE_DF = pd.read_csv(PRE_RACE_DATASET_M2)
        # serve per l'elo pre-gara
        PRE_RACE_DF[["driver", "team"]] = PRE_RACE_DF[["driver", "team"]].astype(str)
        PRE_RACE_DF["elo_driver_pre_race"] = pd.to_numeric(
            PRE_RACE_DF["elo_driver_pre_race"], errors="coerce"
        )
        PRE_RACE_DF["elo_team_pre_race"] = pd.to_numeric(
            PRE_RACE_DF["elo_team_pre_race"], errors="coerce"
        )
        if not PRE_RACE_DF.empty:
            latest = PRE_RACE_DF.sort_values(["anno", "round"]).drop_duplicates(
                ["driver"], keep="last"
            )
            ELO_CACHE["driver"] = (
                latest.set_index("driver")["elo_driver_pre_race"].fillna(1500).to_dict()
            )
            ELO_CACHE["team"] = (
                latest.set_index("team")["elo_team_pre_race"].fillna(1500).to_dict()
            )
        print("✅  Pre‑race DF loaded")

    if CALENDAR_PATH.exists():
        CALENDAR_DF = pd.read_csv(CALENDAR_PATH)
        if "nome_gran_premio" in CALENDAR_DF.columns:
            CALENDAR_DF = CALENDAR_DF.rename(columns={"nome_gran_premio":"gp"})
        print("✅ Calendar loaded – columns:", CALENDAR_DF.columns.tolist())



load_resources()

# ── helper functions ---------------------------------------------------------

# Questa funzione restituisce una lista di Gran Premi per un anno specifico.
def gp_list_for_year(year: int) -> List[str]:
    if year == YEAR_CURR and not CALENDAR_DF.empty:
        df = CALENDAR_DF.copy()
        if "round" in df.columns:
            df = df.sort_values("round")
        # ora esiste sempre la colonna 'gp'
        return df["gp"].dropna().unique().tolist()

    # fallback storico (2023/2024 o 2025 parziale)
    if not ALL_M1_DF.empty:
        rows = ALL_M1_DF[ALL_M1_DF["anno"] == year]
        if not rows.empty:
            return sorted(rows["gp"].unique().tolist())
    return []


# Questa funzione restituisce i dati di gara per un Gran Premio specifico, anno e giro.
def fetch_lap_data(year: int, gp: str, lap: int) -> LapDataM1:
    if ALL_M1_DF.empty:
        return LapDataM1(error="Dataset Modello‑1 non caricato.")
    mask = (
        (ALL_M1_DF["anno"] == year)
        & (ALL_M1_DF["gp"] == gp)
        & (ALL_M1_DF["lap_number"] == lap)
    )
    df = ALL_M1_DF.loc[mask].copy()
    if df.empty:
        return LapDataM1(error=f"Nessun dato per {gp} {year} – giro {lap}.")
    missing = [c for c in FEATURES_M1 if c not in df.columns]
    if missing:
        return LapDataM1(error=f"Feature mancanti: {missing}")
    X = df[FEATURES_M1].copy()
    y = df[TARGET_M1] if TARGET_M1 in df else pd.Series(dtype=int)
    return LapDataM1(X=X, y_true=y, display_raw=df)

# ── UI logic helpers ---------------------------------------------------------

#Questa funzione restituisce gli aggiornamenti per i componenti dell'interfaccia utente
def ui_state_for_gp(year: int, gp: str) -> Tuple[
    gr.Slider.update,
    gr.Button.update,
    gr.Button.update,
    gr.Markdown.update,
    gr.File.update,
    gr.Button.update,
    gr.Dataframe.update,
    gr.Dataframe.update,
]:
    """Centralised logic -> exactly **8** component updates."""
    hide = gr.update(visible=False)
    slider_upd: gr.Slider.update = gr.Slider.update(value=1, minimum=1, maximum=30, visible=False)
    m1_show_upd = gr.Button.update(visible=False)
    m1_pred_upd = gr.Button.update(visible=False)
    md_upd = gr.Markdown.update(visible=False)
    file_upd = gr.File.update(visible=False)
    m2_pred_upd = gr.Button.update(visible=False)
    sit_table_upd = gr.Dataframe.update(visible=False)
    pred_table_reset = gr.Dataframe.update(value=None, visible=True) # Make preds table visible for errors

    if not gp:
        return (
            slider_upd,
            m1_show_upd,
            m1_pred_upd,
            md_upd,
            file_upd,
            m2_pred_upd,
            sit_table_upd,
            pred_table_reset,
        )

    disputed = (year, gp) in DISPUTED_RACES

    if disputed:
        # Modello 1 oer gare passate
        subset = ALL_M1_DF[(ALL_M1_DF["anno"] == year) & (ALL_M1_DF["gp"] == gp)]
        max_lap = 30 # Default
        if not subset.empty and "lap_number" in subset.columns:
            max_lap_val = subset["lap_number"].max()
            if pd.notna(max_lap_val):
                 max_lap = min(int(max_lap_val), 75) # Increased max lap slightly
        else:
             print(f"⚠️  Warning: Could not determine max_lap for {year} {gp}. Defaulting to {max_lap}.")

        slider_upd = gr.Slider.update(visible=True, maximum=max_lap, label=f"Giro (1‑{max_lap})", value=1)
        m1_show_upd = gr.Button.update(visible=True)
        m1_pred_upd = gr.Button.update(value="🔮 Predici Risultati (Mod.1)", visible=True)
        sit_table_upd = gr.Dataframe.update(visible=True)
    else:
        # Gp futuro -> Modello 2
        md_upd = gr.Markdown.update(
            value=f"""
### 🚦 Predizione Pre-Gara per {gp} {year} (Modello 2)
Questo Gran Premio non è ancora stato disputato o non ci sono dati di gara.
Puoi caricare un file `.txt` con la griglia di partenza (un pilota per riga, in ordine di partenza)
per ottenere una predizione pre-gara basata sul Modello 2.
            """,
            visible=True
        )
        file_upd = gr.File.update(label=f"Carica griglia di partenza per {gp} (.txt)", visible=True, value=None)
        m2_pred_upd = gr.Button.update(visible=True, value="🔮 Predici Griglia (Mod.2)")

    return (
        slider_upd,
        m1_show_upd,
        m1_pred_upd,
        md_upd,
        file_upd,
        m2_pred_upd,
        sit_table_upd,
        pred_table_reset, # Resetta la tabella delle predizioni quando cambia il GP
    )


# ── model‑1 UI handlers -------------------------------------------------------

# Questa funzione viene chiamata quando l'utente seleziona un anno e un GP, serve per vedere la situazione ad un dato giro
def show_situation(year: int, gp: str, lap: int):
    if not year or not gp:
        return gr.Dataframe.update(value=pd.DataFrame({"Info": ["Seleziona Anno e Gran Premio."]}))
    data = fetch_lap_data(year, gp, lap)
    if data.error:
        return gr.Dataframe.update(value=pd.DataFrame({"Errore": [data.error]}))
    if data.display_raw is None:
        return gr.Dataframe.update(value=pd.DataFrame({"Errore": ["Dati raw non disponibili per la visualizzazione."]}))


    df_raw = data.display_raw.copy()
    df_disp = pd.DataFrame(index=df_raw.index)

    # position format
    df_raw["position_sort"] = pd.to_numeric(df_raw["position"], errors="coerce").fillna(99)
    df_disp[SITUATION_DISPLAY["position_display"]] = df_raw["position_sort"].apply(
        lambda x: "DNF" if x in (0, 99) else int(x)
    )

    # Restanti colonne
    for src, dst in SITUATION_DISPLAY.items():
        if dst in df_disp.columns:
            continue  
        if src == "gap_leader_display":
            df_disp[dst] = df_raw["gap_to_leader_diff"].apply(
                lambda v: "Leader" if v == 0 else (f"{v:+.3f}s" if pd.notna(v) else "N/D")
            )
        elif src in df_raw.columns:
            if src in ("pit_in", "pit_out"): # booleani
                df_disp[dst] = (
                    df_raw[src]
                    .astype(str) # Assicura che sia stringa
                    .str.lower()
                    .map({"true": "Sì", "1": "Sì", "1.0": "Sì", "false": "No", "0": "No", "0.0": "No"})
                    .fillna("N/D") # Gestisce i valori NaN
                )
            elif src == "lap_time":
                 df_disp[dst] = df_raw[src].apply(lambda x: f"{x:.3f}s" if pd.notna(x) else "N/D")
            else:
                df_disp[dst] = df_raw[src]
        else:
            df_disp[dst] = "N/D"

    df_disp = df_disp.iloc[df_raw["position_sort"].argsort()]
    ordered_cols = [c for c in SITUATION_DISPLAY.values() if c in df_disp.columns]
    return gr.Dataframe.update(value=df_disp[ordered_cols])

# Funzione per predire i risultati di gara (Modello 1)
def predict_m1(year: int, gp: str, lap: int):
    if not year or not gp:
        return gr.Dataframe.update(value=pd.DataFrame({"Info": ["Seleziona Anno e Gran Premio."]}))
    if not model1_lgbm and not model1_cb:
        return gr.Dataframe.update(value=pd.DataFrame({"Errore": ["Modelli M1 non caricati."]}))
    data = fetch_lap_data(year, gp, lap)
    if data.error or data.X is None or data.y_true is None or data.display_raw is None:
        error_msg = data.error if data.error else "Dati incompleti per la predizione M1."
        return gr.Dataframe.update(value=pd.DataFrame({"Errore": [error_msg]}))
    
    X = data.X 
    base = data.display_raw[["driver", "team"]].reset_index(drop=True)
    y_true_series = data.y_true.reset_index(drop=True)
    y_true = y_true_series.fillna(0).astype(int)

    # Assicura che le colonne siano nell'ordine corretto
    preds: list[np.ndarray] = []
    if model1_lgbm is not None:
        try:
            preds.append(model1_lgbm.predict(X))
        except Exception as e:
            print(f"Errore predizione LGBM M1: {e}")
            return gr.Dataframe.update(value=pd.DataFrame({"Errore": [f"LGBM M1: {e}"]} ))
    if model1_cb is not None:
        try:
            preds.append(model1_cb.predict(X))
        except Exception as e:
            print(f"Errore predizione CatBoost M1: {e}")
            return gr.Dataframe.update(value=pd.DataFrame({"Errore": [f"CatBoost M1: {e}"]} ))


    if not preds:
        return gr.Dataframe.update(value=pd.DataFrame({"Errore": ["Nessun modello M1 disponibile o errore predizione."]}))

    raw = np.mean(preds, axis=0)
    order = np.argsort(raw) # Ordina per la posizione prevista
    ranks = np.empty_like(order)
    ranks[order] = np.arange(1, len(raw) + 1) 

    out_df = base.copy()
    out_df["predicted_position"] = ranks
    out_df["actual_position"] = y_true.apply(lambda x: "N/D" if x == 0 else x) # Fa vedere DNF come N/D, se la posizione è 0
    
    # Calcola la differenza tra posizione prevista e reale
    def calculate_diff(row):
        if pd.notna(row["actual_position"]) and isinstance(row["actual_position"], (int, float)) and row["actual_position"] !=0:
            return row["predicted_position"] - row["actual_position"]
        return "N/A" 

    out_df["difference"] = out_df.apply(calculate_diff, axis=1)
    out_df = out_df.sort_values("predicted_position").rename(columns=PRED_M1_DISPLAY)
    
    # Assicura che le colonne siano nell'ordine corretto
    final_cols = [PRED_M1_DISPLAY.get(col_key, col_key) for col_key in ["driver", "team", "predicted_position", "actual_position", "difference"]]
    return gr.Dataframe.update(value=out_df[final_cols])


# ── model‑2 handler -----------------------------------------------------------
#Questa funzione è chiamata quando l'utente preme il pulsante "Predici Griglia (Mod.2)", serve per predire il risultato finale partendo dalla griglia di partenza
def predict_m2(year: int, gp: str, txt_file: Optional[Any]):
    # Controlli vari
    if not year or not gp:
        return gr.Dataframe.update(value=pd.DataFrame({"Info": ["Seleziona Anno e Gran Premio."]}))
    if model2_cb is None:
        return gr.Dataframe.update(value=pd.DataFrame({"Errore": ["Modello‑2 non caricato."]}))
    if txt_file is None:
        return gr.Dataframe.update(value=pd.DataFrame({"Errore": ["Carica un file .txt con la griglia di partenza."]}))
    if PRE_RACE_DF.empty: #Should not happen if load_resources worked
        return gr.Dataframe.update(value=pd.DataFrame({"Errore": ["Dataset pre‑gara (M2) non caricato."]}))

    # read grid order ---------------------------------------------------------
    try:
        with open(txt_file.name, "r", encoding="utf‑8") as f:
            drivers = [ln.strip().upper() for ln in f if ln.strip()]
        if not drivers:
            return gr.Dataframe.update(value=pd.DataFrame({"Errore": ["File griglia vuoto o malformattato."]}))
    except Exception as e:
        return gr.Dataframe.update(value=pd.DataFrame({"Errore": [f"Errore lettura file griglia: {e}"]}))

    # mappa il driver -> la squadra utilizzando i dati più recenti disponibili
    # Si assicura che PRE_RACE_DF ha 'driver' e 'team'
    if not ({'driver', 'team', 'anno', 'round'}.issubset(PRE_RACE_DF.columns)):
        return gr.Dataframe.update(value=pd.DataFrame({"Errore": ["Colonne necessarie mancanti in PRE_RACE_DF."]}))

    recent = (
        PRE_RACE_DF[PRE_RACE_DF["driver"].isin(drivers)]
        .sort_values(["anno", "round"], ascending=[False, False]) # Prende i più recenti
        .drop_duplicates(["driver"], keep="first") 
    )
    d2t = recent.set_index("driver")["team"].to_dict() # Serve per mappare i nomi dei piloti ai team

    # build feature rows ------------------------------------------------------
    rows: list[Dict[str, Any]] = []
    for idx, drv_name_upper in enumerate(drivers, start=1):
        team_name = d2t.get(drv_name_upper, "UnknownTeam") # Default se non trovato
        
        # Inizializza i dati della riga
        row_data: Dict[str, Any] = {
            "starting_grid_position": idx,
            "driver": drv_name_upper,
            "team": team_name,
            "gp": gp, # GP name from dropdown
            "is_driver_new_to_circuit": 1,
        }
        
        # ELO scores
        row_data["elo_driver_pre_race"] = ELO_CACHE.get("driver", {}).get(drv_name_upper, 1500.0)
        row_data["elo_team_pre_race"] = ELO_CACHE.get("team", {}).get(team_name, 1500.0)

        # Statistiche storiche per il pilota in questo GP
        # Filtra PRE_RACE_DF per il pilota e il GP correnti.
        # Nota: le caratteristiche di PRE_RACE_DF si riferiscono alle gare *passate* su questo circuito.
        # Dobbiamo selezionare gli *ultimi dati storici disponibili* per questo pilota *in questo circuito*.
        driver_gp_history = PRE_RACE_DF[
            (PRE_RACE_DF['driver'] == drv_name_upper) & (PRE_RACE_DF['gp'] == gp)
        ].sort_values('anno', ascending=False)

        if not driver_gp_history.empty:
            latest_history = driver_gp_history.iloc[0] # Anno più recente che i piloti hanno corso su questo circuito
            row_data["is_driver_new_to_circuit"] = 0 # Il driver ha già corso su questo circuito

            # Riempie le statistiche storiche
            for col in NUM_COLS_M2:
                if col in latest_history and pd.notna(latest_history[col]):
                    row_data[col] = latest_history[col]
                elif col not in row_data: # Se non è stato impostato da grid pos o ELO
                    row_data[col] = 0 # Default per statistiche storiche non disponibili
        else:
            # Il pilota non ha mai corso su questo circuito
            row_data["is_driver_new_to_circuit"] = 1
            for col in NUM_COLS_M2:
                if col not in row_data: # Se non è stato impostato da grid pos o ELO
                    # Per statistiche storiche specifiche, 0 potrebbe essere appropriato, o un valore neutro/medio.
                    # Per esempio, avg_finish_pos_circuit potrebbe essere un numero alto, come 20, se è nuovo.
                    if "pos" in col: row_data[col] = 20 # Per esempio, 20 come posizione finale media
                    elif "pct" in col: row_data[col] = 0.5 # 50%
                    else: row_data[col] = 0
        
        rows.append(row_data)

    X_pred = pd.DataFrame(rows)
    # Ci assicura che le colonne siano nell'ordine corretto
    for col in FEATURES_M2:
        if col not in X_pred.columns:
            if col in NUM_COLS_M2:
                X_pred[col] = 0 # Default per colonne numeriche
            else: # Categorical
                X_pred[col] = "Unknown"
        elif col in NUM_COLS_M2: # Assicura che le colonne numeriche siano float
             X_pred[col] = X_pred[col].fillna(0)


    try:
        ranks_raw = model2_cb.predict(X_pred[FEATURES_M2])
    except Exception as e:
        print(f"Errore predizione CatBoost M2: {e}")
        
        missing_model_cols = [f for f in FEATURES_M2 if f not in X_pred.columns]
        if missing_model_cols:
            return gr.Dataframe.update(value=pd.DataFrame({"Errore": [f"Mancano colonne per M2: {missing_model_cols}. Errore: {e}"]}))
        return gr.Dataframe.update(value=pd.DataFrame({"Errore": [f"CatBoost M2: {e}. Controlla i tipi di dati e i valori."]}))


    order = np.argsort(ranks_raw) # Ordina le posizioni previste
    ranks = np.empty_like(order)
    ranks[order] = np.arange(1, len(ranks_raw) + 1)

    out_df = X_pred[["driver", "team"]].copy()
    out_df["predicted_position"] = ranks
    out_df = out_df.sort_values("predicted_position").rename(columns=PRED_M2_DISPLAY)
    
    final_cols_m2 = [PRED_M2_DISPLAY.get(col_key, col_key) for col_key in ["driver", "team", "predicted_position"]]
    return gr.Dataframe.update(value=out_df[final_cols_m2])


# ── gradio UI ----------------------------------------------------------------
CSS = """
:root {
  --background-fill-primary: #282c34 !important;
  --background-fill-secondary: #21252b !important;
  --border-color-primary: #3d424c;
  --color-text-primary: #abb2bf; 
  --color-text-secondary: #89909e; 
  --accent-color: #e10600 !important;
  --block-title-text-color: #e10600 !important; 
  --input-background-fill: #21252b !important; 
  --input-border-color: #3d424c !important;
  --input-text-color: #abb2bf !important; 
}
body, .gradio-container {
  background-color: var(--background-fill-primary) !important;
  color: var(--color-text-primary) !important;
  font-family: 'Orbitron', sans-serif; 
}
.gr-button {
  border-radius: 8px;
  font-weight: 600; /* Slightly bolder */
  background-color: var(--accent-color) !important;
  color: white !important;
  border: none !important;
  transition: background-color 0.3s ease; 
}
.gr-button:hover {
  background-color: #c00500 !important; 
}
.gr-button.gr-button-secondary {
  background-color: var(--background-fill-secondary) !important;
  color: var(--color-text-primary) !important;
  border: 1px solid var(--border-color-primary) !important;
}
.gr-button.gr-button-secondary:hover {
  background-color: #3a3f47 !important; 
  border-color: var(--accent-color) !important;
}
.gr-input, .gr-dropdown, .gr-textbox, .gr-slider > .svelte-jsloader  { /* Target slider rail */
    background-color: var(--input-background-fill) !important;
    color: var(--input-text-color) !important;
    border: 1px solid var(--border-color-primary) !important;
}
.gr-dropdown select, .gr-textbox textarea {
    background-color: var(--input-background-fill) !important;
    color: var(--input-text-color) !important;
}
.gr-slider input[type=range]::-webkit-slider-thumb {
    background: var(--accent-color) !important; 
}
.gr-slider input[type=range]::-moz-range-thumb {
    background: var(--accent-color) !important;
}
.gr-data-frame { /* Styling for dataframes */
    border: 1px solid var(--border-color-primary);
    border-radius: 8px;
    overflow: hidden; /* For rounded corners on table */
}
.gr-data-frame table th {
    background-color: var(--accent-color) !important; 
    color: white !important;
    font-weight: 600;
}
.gr-data-frame table td {
    background-color: var(--background-fill-secondary) !important;
    color: var(--color-text-primary) !important;
    border-bottom: 1px solid var(--border-color-primary);
}
.gr-block-label { /* For labels like "Anno", "Gran Premio" */
    color: var(--color-text-secondary) !important;
    font-weight: 500;
}
.formula-ai-title {
    font-size: 4em; /* Larger font size */
    font-weight: 700;
    color: #FFFFFF !important; 
    text-align: center;
    margin-bottom: 20px;
    text-shadow: 2px 2px 8px var(--accent-color); 
}
.app-main-title {
    color: var(--accent-color) !important;
    text-align: center;
    margin-bottom: 10px;
    font-size: 2.5em;
}
.gr-tab-item.selected {
    background-color: var(--accent-color) !important;
    color: white !important;
    border-bottom: 2px solid var(--accent-color) !important;
}
.gr-tab-item {
    color: var(--color-text-primary) !important;
    background-color: var(--background-fill-secondary) !important;
}


#video-warning-box h3 {
    color: var(--accent-color);
    margin-bottom: 10px;
}
#video-warning-box code {
    background-color: var(--background-fill-secondary);
    padding: 2px 5px;
    border-radius: 3px;
    color: var(--color-text-primary);
}

#dd_year_id .gr-block-label > span,
#dd_gp_id .gr-block-label > span,
#file_grid_id .gr-block-label > span,
#slider_lap_id .gr-block-label > span {
    background-color: var(--accent-color) !important;
    color: white !important;
    padding: 4px 10px !important; 
    border-radius: 5px !important; 
    font-weight: 600 !important; 
    display: inline-block !important;
    margin-bottom: 5px !important; 
    box-shadow: 0 2px 4px rgba(0,0,0,0.2); 
}

/* Stile per il titolo Markdown H3 dentro il gruppo M2 */
#md_grid_id h3 {
    background-color: var(--accent-color);
    color: white;
    padding: 6px 12px;
    border-radius: 5px;
    display: inline-block; /* Per far sì che il background si adatti al testo */
    margin-bottom: 12px;
    font-size: 1.1em; /* Leggermente più grande */
    font-weight: 600;
    box-shadow: 0 2px 4px rgba(0,0,0,0.2);
}

/* Stile per il componente File Upload per renderlo meno "vuoto" */
#file_grid_id .gr-file-input, #file_grid_id .gr-upload-button {
    border-radius: 6px;
}
#file_grid_id .gr-file-input { /* Area di drop */
    border: 2px dashed var(--border-color-primary) !important;
    background-color: rgba(40, 44, 52, 0.7) !important; /* Sfondo leggermente più scuro */
    min-height: 120px; /* Altezza minima per l'area di drop */
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
}
#file_grid_id .gr-file-input:hover {
    border-color: var(--accent-color) !important;
}
#file_grid_id .gr-upload-button.gr-button-secondary { /* Bottone "Click to Upload" */
    background-color: var(--background-fill-secondary) !important;
    color: var(--color-text-primary) !important;
    border: 1px solid var(--border-color-primary) !important;
    font-weight: 500;
}
#file_grid_id .gr-upload-button.gr-button-secondary:hover {
  background-color: #3a3f47 !important;
  border-color: var(--accent-color) !important;
}

/* Opzionale: per spaziare il bottone M2 se necessario */
.button-spacer-m2 {
    min-height: 20px; 
    
}
#btn_pred_m2_id {
    margin-top: 15px !important; 
    display: block !important; /* Assicura che occupi la sua riga */
    margin-left: auto !important; /* Centra il bottone se la colonna è più larga */
    margin-right: auto !important; /* Centra il bottone se la colonna è più larga */
    max-width: 300px; /* Limita la larghezza massima del bottone */
}

/* Colore testo specifico per il markdown in group_m2, escluso l'H3 già stilato */
#md_grid_id p {
    color: var(--color-text-primary) !important; /* Assicura che il testo del paragrafo sia leggibile */
    line-height: 1.5;
}
.qr-center {
  display: block;
  margin-left: auto;
  margin-right: auto;
}

"""


theme = gr.themes.Soft(
    primary_hue=gr.themes.colors.red,
    secondary_hue=gr.themes.colors.neutral,
    neutral_hue=gr.themes.colors.gray,
    font=[gr.themes.GoogleFont("Orbitron"), "ui-sans-serif", "system-ui", "sans-serif"],
).set(

     body_background_fill="*background_fill_primary",
     block_title_text_color="*accent_color",
     button_primary_background_fill="*accent_color",
     button_primary_text_color="white",
)


with gr.Blocks(
    title="Formula AI Predictor",
    theme=theme, # Use the defined theme
    css=CSS,
) as demo:
    with gr.Tabs(): 
        with gr.TabItem("🏁 Home", id="home_tab"):
            # Titolo grande
            gr.Markdown(
                "<h1 class='formula-ai-title'>Formula AI</h1>"
            )
            # Breve introduzione
            gr.Markdown(
    """
# 🎉 Benvenuti in **Formula AI**!

Siamo due studenti de **La Sapienza di Roma** e questo progetto nasce per il corso di **AiLab di Informatica**.

**Obiettivo:**  
Utilizzare l’Intelligenza Artificiale per fare predizioni sulle gare di **Formula 1**.

---

## 🏁 I nostri modelli

1. **Modello 1** – Analisi in tempo reale di una gara in corso (o già disputata)  
2. **Modello 2** – Predizione _pre-gara_ basata sulla griglia di partenza

---

🚀 **Vai subito nella tab “Predictor” per cominciare!**

---

#### 🔗 Repository GitHub  
Scansiona il QR code qui sotto per esplorare il nostro codice, documentazione e le ultime novità!
"""
)   
            #QR code
            gr.HTML(
        """
        <div style="text-align:center; margin-top:30px;">
          <img 
            src="/file=QRCodeAiLab.png" 
            alt="QR Code" 
            style="width:200px; height:200px; display:inline-block;"
          />
        </div>
        """
    )
            

        # Tab per il Predictor
        with gr.TabItem("🚀 Predictor", id="predictor_tab"):
            gr.Markdown("# 🏎️ Formula AI Predictor", elem_classes=["app-main-title"])
            gr.Markdown("### Analisi e Predizioni per Gran Premi Disputati e Futuri")

            # --- selection row ------------------------------------------------------
            with gr.Row(variant="panel"):
                dd_year = gr.Dropdown(GP_YEARS, label="Anno", value=YEAR_CURR, elem_id="dd_year_id")
                dd_gp = gr.Dropdown([], label="Gran Premio", interactive=True, elem_id="dd_gp_id")

            with gr.Group(visible=False) as group_m1: # Group for M1 controls
                gr.Markdown("#### Modello 1: Analisi Gara in Corso / Disputata")
                slider_lap = gr.Slider(1, 75, label="Giro", step=1, interactive=True, elem_id="slider_lap_id") # Aumenta max lap
                with gr.Row():
                    btn_situation = gr.Button("📊 Mostra Situazione Giro", elem_id="btn_situation_id")
                    btn_pred_m1 = gr.Button("🔮 Predici Risultati (Mod.1)", elem_id="btn_pred_m1_id")
            
            with gr.Group(visible=False) as group_m2: # Gruppo per M2
                md_grid = gr.Markdown(elem_id="md_grid_id") # Contenuto caricato dinamicamente
                with gr.Column(): # Una colonna per contenere entrambi verticalmente
                    file_grid = gr.File(file_types=[".txt"], label="Carica griglia di partenza (.txt)", elem_id="file_grid_id")
                    btn_pred_m2 = gr.Button("🔮 Predici Griglia (Mod.2)", elem_id="btn_pred_m2_id", scale=0) # scale=0 per dimensione automatica

            gr.Markdown("---") # Separatore Markdown
            
            tbl_situation = gr.Dataframe(label="Situazione Giro Corrente/Selezionato", interactive=False, visible=False, wrap=True, height=400)
            tbl_preds = gr.Dataframe(label="Risultati Predizioni", interactive=False, wrap=True, height=400)


            # -------- logic wires ---------------------------------------------------
            #Questa funzione viene chiamata quando l'anno cambia, per aggiornare la lista dei GP e lo stato dell'interfaccia utente.
            def _update_gp_and_ui_visibility(year: int):
                gps = gp_list_for_year(year)
                selected_gp = gps[0] if gps else None
                
                # Get UI state based on the newly selected GP (or lack thereof)
                (slider_upd, m1_show_btn_upd, m1_pred_btn_upd, 
                 md_grid_upd, file_grid_upd, m2_pred_btn_upd, 
                 sit_tbl_upd, pred_tbl_upd) = ui_state_for_gp(year, selected_gp)

                # Determine visibility of M1 and M2 groups
                is_disputed_gp = (year, selected_gp) in DISPUTED_RACES if selected_gp else False
                m1_group_visible = gr.update(visible=is_disputed_gp)
                m2_group_visible = gr.update(visible=not is_disputed_gp and selected_gp is not None)

                return (
                    gr.Dropdown.update(choices=gps, value=selected_gp),
                    m1_group_visible, # group_m1 visibility
                    m2_group_visible, # group_m2 visibility
                    slider_upd,
                    m1_show_btn_upd,
                    m1_pred_btn_upd,
                    md_grid_upd,
                    file_grid_upd,
                    m2_pred_btn_upd,
                    sit_tbl_upd,
                    pred_tbl_upd,
                )
            # Questa funzione viene chiamata quando l'anno cambia, per aggiornare la lista dei GP e lo stato dell'interfaccia utente.
            def _update_ui_for_gp_change(year: int, gp_name: str):
                (slider_upd, m1_show_btn_upd, m1_pred_btn_upd, 
                 md_grid_upd, file_grid_upd, m2_pred_btn_upd, 
                 sit_tbl_upd, pred_tbl_upd) = ui_state_for_gp(year, gp_name)

                is_disputed_gp = (year, gp_name) in DISPUTED_RACES if gp_name else False
                m1_group_visible = gr.update(visible=is_disputed_gp)
                m2_group_visible = gr.update(visible=not is_disputed_gp and gp_name is not None)
                
                return (
                    m1_group_visible, # group_m1 
                    m2_group_visible, # group_m2 
                    slider_upd,
                    m1_show_btn_upd,
                    m1_pred_btn_upd,
                    md_grid_upd,
                    file_grid_upd,
                    m2_pred_btn_upd,
                    sit_tbl_upd,
                    pred_tbl_upd,
                )

            # Reagisce quando l'anno cambia
            dd_year.change(
                _update_gp_and_ui_visibility,
                inputs=[dd_year],
                outputs=[
                    dd_gp, group_m1, group_m2,
                    slider_lap, btn_situation, btn_pred_m1,
                    md_grid, file_grid, btn_pred_m2,
                    tbl_situation, tbl_preds
                ]
            )

            # Reagisce quando il GP cambia
            dd_gp.change(
                _update_ui_for_gp_change,
                inputs=[dd_year, dd_gp],
                outputs=[
                    group_m1, group_m2,
                    slider_lap, btn_situation, btn_pred_m1,
                    md_grid, file_grid, btn_pred_m2,
                    tbl_situation, tbl_preds
                ]
            )
            
            # actions ----------------------------------------------------------------
            btn_situation.click(show_situation, [dd_year, dd_gp, slider_lap], tbl_situation)
            btn_pred_m1.click(predict_m1, [dd_year, dd_gp, slider_lap], tbl_preds)
            btn_pred_m2.click(predict_m2, [dd_year, dd_gp, file_grid], tbl_preds)

            # carica per la scheda predittore

            # Vogliamo inizializzare la tendina GP e la visibilità dell'interfaccia utente in base all'anno predefinito.
            #La funzione _init_predictor_tab viene chiamata quando il demo viene caricato.
            def _init_predictor_tab(initial_year: int):
                
                return _update_gp_and_ui_visibility(initial_year)

            demo.load(
                _init_predictor_tab,
                inputs=dd_year, 
                outputs=[
                    dd_gp, group_m1, group_m2,
                    slider_lap, btn_situation, btn_pred_m1,
                    md_grid, file_grid, btn_pred_m2,
                    tbl_situation, tbl_preds
                ],
            )

if __name__ == "__main__":
    
    demo.launch(debug=True, share=False)