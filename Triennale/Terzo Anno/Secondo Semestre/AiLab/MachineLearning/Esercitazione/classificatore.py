import cv2
import os

# === CONFIG ===
DATABASE_PATH = "Data/database"
MATCH_THRESHOLD = 10  # soglia minima di match per considerare una classificazione valida

# === CREA DATABASE DI DESCRITTORI ===
def build_database():
    orb = cv2.ORB_create()
    database = []

    for filename in os.listdir(DATABASE_PATH):
        path = os.path.join(DATABASE_PATH, filename)
        img = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
        if img is None:
            print(f"Immagine non valida: {path}")
            continue
        kp, des = orb.detectAndCompute(img, None)
        if des is not None:
            database.append((filename, des))
        else:
            print(f"Nessun descrittore trovato in {filename}")
    return database

# === CLASSIFICA IMMAGINE TEST ===
def classify_image(test_img, database):
    orb = cv2.ORB_create()
    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
    _, test_des = orb.detectAndCompute(test_img, None)

    if test_des is None:
        print("Nessun descrittore nella test image.")
        return None, 0

    match_counts = []
    for name, des in database:
        matches = bf.match(des, test_des)
        matches = sorted(matches, key=lambda x: x.distance)
        match_counts.append((name, len(matches)))

    best_match = max(match_counts, key=lambda x: x[1])
    if best_match[1] >= MATCH_THRESHOLD:
        return best_match[0], best_match[1]
    else:
        return None, best_match[1]

# === ACQUISISCI DA WEBCAM E CLASSIFICA ===
def run_realtime_classification():
    print("Costruisco il database delle immagini...")
    database = build_database()
    if not database:
        print("Database vuoto. Aggiungi immagini in /database.")
        return

    print("Premi 'q' per uscire.")
    cap = cv2.VideoCapture(0)

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Errore nella webcam.")
            break

        gray = cv2.cvtColor(frame, cv2.COLOR_BGRA2GRAY)
        label, score = classify_image(gray, database)

        display_text = f"Classe: {label} ({score} match)" if label else f"Nessuna corrispondenza ({score} match)"
        cv2.putText(frame, display_text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255,0,0) if label else (0,0,255), 2)

        cv2.imshow("Classificazione oggetto", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

# === MAIN ===
if __name__ == "__main__":
    run_realtime_classification()
