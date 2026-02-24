#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

// Precisione da utilizzare per i calcoli
#define fptype float

// Numero di esecuzioni per misurare le prestazioni
#define NUM_RUNS 1000
#include <math.h>
#include <stdio.h>

// Definizione di una tolleranza per i confronti
#define TOLERANCE 1e-4

// Funzione per verificare i risultati
void verify_results(fptype *parallel_prices, const char *serial_output_file, int numOptions) {
    FILE *file = fopen(serial_output_file, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s for verification.\n", serial_output_file);
        return;
    }

    // Carica i risultati sequenziali
    int serial_numOptions;
    fptype *serial_prices = (fptype *)malloc(numOptions * sizeof(fptype));
    fscanf(file, "%d", &serial_numOptions);
    if (serial_numOptions != numOptions) {
        printf("Error: Number of options in serial output does not match!\n");
        fclose(file);
        free(serial_prices);
        return;
    }

    for (int i = 0; i < numOptions; i++) {
        fscanf(file, "%f", &serial_prices[i]);
    }
    fclose(file);

    // Confronta i risultati paralleli con quelli sequenziali
    int mismatches = 0;
    for (int i = 0; i < numOptions; i++) {
        if (fabs(parallel_prices[i] - serial_prices[i]) > TOLERANCE) {
            printf("Mismatch at option %d: Parallel=%.6f, Serial=%.6f\n", i, parallel_prices[i], serial_prices[i]);
            mismatches++;
        }
    }

    if (mismatches == 0) {
        printf("Verification successful: All results match the serial implementation.\n");
    } else {
        printf("Verification failed: Found %d mismatches.\n", mismatches);
    }

    free(serial_prices);
}

// Struttura per rappresentare i dati di ogni opzione
typedef struct OptionData_ {
    fptype s;          // Prezzo spot (prezzo corrente dell'asset)
    fptype strike;     // Prezzo di esercizio
    fptype r;          // Tasso di interesse privo di rischio
    fptype divq;       // Tasso di dividendo (non utilizzato)
    fptype v;          // Volatilità
    fptype t;          // Tempo fino alla scadenza (in anni)
    char OptionType;   // Tipo di opzione: "P"=Put, "C"=Call
    fptype divs;       // Dividendi (non utilizzati)
    fptype DGrefval;   // Valore di riferimento per verifica
} OptionData;

// Variabili globali per input e risultati
OptionData *data;     // Dati delle opzioni
fptype *prices;       // Prezzi calcolati
int numOptions;       // Numero totale di opzioni

// Variabili ausiliarie per i calcoli
int *otype;           // Tipo di opzione (0=Call, 1=Put)
fptype *sptprice;     // Prezzi spot
fptype *strike;       // Prezzi di esercizio
fptype *rate;         // Tassi di interesse
fptype *volatility;   // Volatilità
fptype *otime;        // Tempo fino alla scadenza

// Numero di thread
int nThreads;

// Costante per la funzione cumulativa della distribuzione normale
#define inv_sqrt_2xPI 0.39894228040143270286

// Funzione cumulativa della distribuzione normale
fptype CNDF(fptype InputX) {
    int sign;
    fptype OutputX, xInput, xNPrimeofX, expValues;
    fptype xK2, xK2_2, xK2_3, xK2_4, xK2_5, xLocal, xLocal_1, xLocal_2, xLocal_3;

    if (InputX < 0.0) {
        InputX = -InputX;
        sign = 1;
    } else {
        sign = 0;
    }

    xInput = InputX;
    expValues = exp(-0.5f * InputX * InputX);
    xNPrimeofX = expValues * inv_sqrt_2xPI;

    xK2 = 0.2316419 * xInput;
    xK2 = 1.0 / (1.0 + xK2);
    xK2_2 = xK2 * xK2;
    xK2_3 = xK2_2 * xK2;
    xK2_4 = xK2_3 * xK2;
    xK2_5 = xK2_4 * xK2;

    xLocal_1 = xK2 * 0.319381530;
    xLocal_2 = xK2_2 * (-0.356563782) + xK2_3 * 1.781477937 + xK2_4 * (-1.821255978) + xK2_5 * 1.330274429;
    xLocal = xNPrimeofX * (xLocal_1 + xLocal_2);
    xLocal = 1.0 - xLocal;

    if (sign) OutputX = 1.0 - xLocal;
    else OutputX = xLocal;

    return OutputX;
}

// Formula di Black-Scholes per calcolare il prezzo di un'opzione europea
fptype BlkSchlsEqEuroNoDiv(fptype sptprice, fptype strike, fptype rate, fptype volatility, fptype time, int otype) {
    fptype xRiskFreeRate = rate;
    fptype xVolatility = volatility;
    fptype xTime = time;
    fptype xSqrtTime = sqrt(xTime);

    fptype logValues = log(sptprice / strike);
    fptype xPowerTerm = xVolatility * xVolatility * 0.5;
    fptype xD1 = (xRiskFreeRate + xPowerTerm) * xTime + logValues;
    fptype xDen = xVolatility * xSqrtTime;
    xD1 = xD1 / xDen;
    fptype xD2 = xD1 - xDen;

    fptype NofXd1 = CNDF(xD1);
    fptype NofXd2 = CNDF(xD2);

    fptype FutureValueX = strike * exp(-rate * time);
    if (otype == 0) {
        return sptprice * NofXd1 - FutureValueX * NofXd2;
    } else {
        return FutureValueX * (1.0 - NofXd2) - sptprice * (1.0 - NofXd1);
    }
}

// Funzione eseguita da ciascun thread
void *bs_thread(void *tid_ptr) {
    int tid = *(int *)tid_ptr;                // Identificatore del thread
    int start = tid * (numOptions / nThreads); // Inizio del lavoro per il thread
    int end = (tid == nThreads - 1) ? numOptions : start + (numOptions / nThreads);

    for (int j = 0; j < NUM_RUNS; j++) {      // NUM_RUNS per ripetere il calcolo
        for (int i = start; i < end; i++) {   // Ciclo su tutte le opzioni assegnate
            prices[i] = BlkSchlsEqEuroNoDiv(sptprice[i], strike[i], rate[i], volatility[i], otime[i], otype[i]);
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: %s <nThreads> <inputFile> <outputFile>\n", argv[0]);
        return 1;
    }

    nThreads = atoi(argv[1]);
    char *inputFile = argv[2];
    char *outputFile = argv[3];

    FILE *file = fopen(inputFile, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", inputFile);
        return 1;
    }

    fscanf(file, "%d", &numOptions);
    data = (OptionData *)malloc(numOptions * sizeof(OptionData));
    prices = (fptype *)malloc(numOptions * sizeof(fptype));

    for (int i = 0; i < numOptions; i++) {
        fscanf(file, "%f %f %f %f %f %f %c %f %f", &data[i].s, &data[i].strike, &data[i].r,
               &data[i].divq, &data[i].v, &data[i].t, &data[i].OptionType, &data[i].divs, &data[i].DGrefval);
    }
    fclose(file);

    sptprice = (fptype *)malloc(5 * numOptions * sizeof(fptype));
    strike = sptprice + numOptions;
    rate = strike + numOptions;
    volatility = rate + numOptions;
    otime = volatility + numOptions;
    otype = (int *)malloc(numOptions * sizeof(int));

    for (int i = 0; i < numOptions; i++) {
        otype[i] = (data[i].OptionType == 'P') ? 1 : 0;
        sptprice[i] = data[i].s;
        strike[i] = data[i].strike;
        rate[i] = data[i].r;
        volatility[i] = data[i].v;
        otime[i] = data[i].t;
    }

    if (nThreads > numOptions) {
        printf("WARNING: Reducing threads to match number of options.\n");
        nThreads = numOptions;
    }

    pthread_t threads[nThreads];
    int tids[nThreads];

    // Misurazione del tempo di esecuzione
    double start = omp_get_wtime();

    // Creazione dei thread
    for (int i = 0; i < nThreads; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, bs_thread, &tids[i]);
    }

    // Attesa della terminazione di tutti i thread
    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    double stop = omp_get_wtime();
    printf("Execution Time: %f seconds\n", stop - start);
    verify_results(prices, "outputs/output_serial.txt", numOptions);
    // Scrittura dei risultati nel file di output
    file = fopen(outputFile, "w");
    if (!file) {
        printf("Error: Unable to open file %s\n", outputFile);
        return 1;
    }
    fprintf(file, "%d\n", numOptions);
    for (int i = 0; i < numOptions; i++) {
        fprintf(file, "%.18f\n", prices[i]);
    }
    fclose(file);

    free(data);
    free(prices);
    free(sptprice);
    free(otype);

    return 0;
}
