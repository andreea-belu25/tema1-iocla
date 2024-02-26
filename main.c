#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"
#define SIZE 50000

void *init_info_senzor(int sensor_type, FILE *read)
{
	//  initializari senzor PMU
	float voltage = 0, current = 0, power_consumption = 0;
	int energy_regen = 0, energy_storage = 0;

	//  initializari senzor TIRE
	float pressure = 0, temperature = 0;
	int wear_level = 0, performance_score = 0;

	//  aloc o zona de memorie de SIZE * sizeof(char) octeti
	void *data = calloc(SIZE, sizeof(char));
	if (!data) //  verific daca alocarea a reusit
		return 0;
	//  data si idx vor pointa catre aceeasi zona de memorie
	//  la idx nu mai aloc o zona de memorie
	// dar, pe idx il deplasez, data va pointa catre inceputul zonei de mem
	void *idx = data;

	if (sensor_type == 1) { //  senzor de tip PMU
		//  citesc caracteristicile acestuia
		fread(&voltage, sizeof(float), 1, read);
		fread(&current, sizeof(float), 1, read);
		fread(&power_consumption, sizeof(float), 1, read);
		fread(&energy_regen, sizeof(int), 1, read);
		fread(&energy_storage, sizeof(int), 1, read);

		//  pun caracteristicile acestuia in idx, in fct de tipul lor de date
		*(float *)idx = voltage;
		idx += sizeof(float);

		*(float *)idx = current;
		idx += sizeof(float);

		*(float *)idx = power_consumption;
		idx += sizeof(float);

		*(int *)idx = energy_regen;
		idx += sizeof(int);

		*(int *)idx = energy_storage;
		idx += sizeof(int);
	} else {//  senzor de tip Tire
		//  citesc caracteristicile acestuia
		fread(&pressure, sizeof(float), 1, read);
		fread(&temperature, sizeof(float), 1, read);
		fread(&wear_level, sizeof(int), 1, read);
		fread(&performance_score, sizeof(int), 1, read);

		//  pun caracteristicile acestuia in idx, in fct de tipul lor de date
		*(float *)idx = pressure;
		idx += sizeof(float);

		*(float *)idx = temperature;
		idx += sizeof(float);

		*(int *)idx = wear_level;
		idx += sizeof(int);

		*(int *)idx = performance_score;
		idx += sizeof(int);
	}
	//  idx ajunge dupa parcurgere sa pointeze la o zona de memorie cu 0
	//  data ramane la inceputul zonei de mem alocate in care am pus val
	return data;
}

void print(void *trimite, int sensor_type)
{
	//  initializari senzor PMU
	float voltage = 0, current = 0, power_consumption = 0;
	int energy_regen = 0, energy_storage = 0;

	//  initializari senzor TIRE
	float pressure = 0, temperature = 0;
	int wear_level = 0, performance_score = 0;

	if (sensor_type == 1) {//  senzor de tip PMU
		//  aflu caracteristicile senzorului PMU
		voltage = *(float *)trimite;
		trimite += sizeof(float);

		current = *(float *)trimite;
		trimite += sizeof(float);

		power_consumption = *(float *)trimite;
		trimite += sizeof(float);

		energy_regen = *(int *)trimite;
		trimite += sizeof(int);

		energy_storage = *(int *)trimite;
		trimite += sizeof(int);

		//  afisez caracteristicile
		printf("Power Management Unit\n");
		printf("Voltage: %.2f\n", voltage);
		printf("Current: %.2f\n", current);
		printf("Power Consumption: %.2f\n", power_consumption);
		printf("Energy Regen: %d%%\n", energy_regen);
		printf("Energy Storage: %d%%\n", energy_storage);
	} else { //  senzor de tip Tire
		//  aflu caracteristicile senzorului TIRE
		pressure = *(float *)trimite;
		trimite += sizeof(float);

		temperature = *(float *)trimite;
		trimite += sizeof(float);

		wear_level = *(int *)trimite;
		trimite += sizeof(int);

		performance_score = *(int *)trimite;
		trimite += sizeof(int);

		//  afisez caracteristicile
		printf("Tire Sensor\n");
		printf("Pressure: %.2f\n", pressure);
		printf("Temperature: %.2f\n", temperature);
		printf("Wear Level: %d%%\n", wear_level);
		if (performance_score == 0)
			printf("Performance Score: Not Calculated\n");
		else
			printf("Performance Score: %d\n", performance_score);
	}
}

int check(void *trimite, int sensor_type) // fct pentru clear
{
	//  caracterisitici care imi indica daca senzorul este eronat sau nu (PMU)
	float voltage = 0, current = 0, power_consumption = 0;
	int energy_regen = 0, energy_storage = 0;

	//  caracterisitici care imi indica daca senzorul este eronat sau nu (Tire)
	float pressure = 0, temperature = 0;
	int wear_level = 0;

	if (sensor_type == 1) { //  senzor de tip PMU
	//  verific daca senzorul este eronat
		voltage = *(float *)trimite;
		if (voltage < 10 || voltage > 20)
			return 0;
		trimite += sizeof(float);

		current = *(float *)trimite;
		if (current < -100 || current > 100)
			return 0;
		trimite += sizeof(float);

		power_consumption = *(float *)trimite;
		if (power_consumption < 0 || power_consumption > 1000)
			return 0;
		trimite += sizeof(float);

		energy_regen = *(int *)trimite;
		if (energy_regen < 0 || energy_regen > 100)
			return 0;
		trimite += sizeof(int);

		energy_storage = *(int *)trimite;
		if (energy_storage < 0 || energy_storage > 100)
			return 0;
		trimite += sizeof(int);
	} else { //  senzor de tip Tire
	//  verific daca senzorul este eronat
		pressure = *(float *)trimite;
		if (pressure < 19 || pressure > 28)
			return 0;
		trimite += sizeof(float);

		temperature = *(float *)trimite;
		if (temperature < 0 || temperature > 120)
			return 0;
		trimite += sizeof(float);

		wear_level = *(int *)trimite;
		if (wear_level < 0 || wear_level > 100)
			return 0;
		trimite += sizeof(int);

	}
	// am returnat 0 daca senzorul este eronat
	return 1; // senzorul nu este eronat
}

void analyze(void *trimite, int operator, void (*operation[])())
{
	//  fiecare fct are ca param un void *
	//  operation[operator] fct care trebuie apelata
	operation[operator](trimite);
}
int main(int argc, char const *argv[])
{
	int nr_senzori = 0, sensor_type = 0, nr_operations = 0, operation = 0;
	int t = 0;
	char string[25] = "\0";
	//  argv[0] = executabilul, argv[1] = fisierul binar
	FILE *read = fopen(argv[1], "rb");
	//  citirea nu s-a realizat cu succes
	if (!read)
		return 0;

	fread(&nr_senzori, sizeof(int), 1, read);

	// void = tipul fct poate fi oricare(double, int etc)
	// () -orice argumente + indica vectorul de pointeri la fct
	// ops = identificatorul
	// 8 = nr de functii
	// * fiecare elem din vectorul ops este un pointer la o fct
	void (*op[8])();
	// pt vectorul de pointer la fct nu se fac alocari!!!
	get_operations((void **)op); // initializare vector de pointeri la fct
	// + 10 => adaug spatiu suplimentar pentru vectori
	int k = 0; //  nr de elem din Senzori
	int k1 = 0; //  nr de elem din Senzori1
	sensor *S = calloc(nr_senzori + 10, sizeof(sensor));
	if (!S)
		return 0;
	sensor *S1 = calloc(nr_senzori + 10, sizeof(sensor));
	if (!S1)
		return 0;

	for (int i = 0; i < nr_senzori; i++) {
		//  initializare informatie in fct de tipul senzorului
		fread(&sensor_type, sizeof(int), 1, read);
		void *info = init_info_senzor(sensor_type, read);
		//  adaug toate campurile in Senzori pentru elem de pe poz k, tip PMU
		if (sensor_type == 1) {
			S[k].sensor_data = info;
			//  sensor_type e de tip enum (Tire, PMU)
			S[k].sensor_type = PMU;
			//  pentru fiecare senzor citesc nr de operatii
			fread(&nr_operations, sizeof(int), 1, read);
			S[k].nr_operations = nr_operations;
			//  alocare pentru vectorul de operatii al fiecarui senzor
			S[k].operations_idxs = calloc(nr_operations, sizeof(int));
			if (!S[k].operations_idxs) //  verific alocarea
				return 0;
			for (int j = 0; j < nr_operations; j++) {
				fread(&operation, sizeof(int), 1, read);
				S[k].operations_idxs[j] = operation;
			}
			k++;
		} else {
			//  adaug campurile in Senzori1 pentru elem de pe poz k1, tip TIRE
			S1[k1].sensor_data = info;
			//  sensor_type e de tip enum (Tire, PMU)
			S1[k1].sensor_type = TIRE;
			//  pentru fiecare senzor citesc nr de operatii
			fread(&nr_operations, sizeof(int), 1, read);
			//  alocare pentru vectorul de operatii al fiecarui senzor
			S1[k1].operations_idxs = calloc(nr_operations, sizeof(int));
			if (!S1[k1].operations_idxs) //  verific alocarea
				return 0;
			S1[k1].nr_operations = nr_operations;
			for (int j = 0; j < nr_operations; j++) {
				fread(&operation, sizeof(int), 1, read);
				S1[k1].operations_idxs[j] = operation;
			}
			k1++;
		}
	}
	//  concatenez vectorul Senzori1 la vectorul Senzori
	//  dupa parcurgerea fisierului binar, nu ma mai pot intoarce la el
	// vector = pointer
	for (int i = 0; i < k1; i++) {
		S[k].sensor_data = S1[i].sensor_data;
		S[k].sensor_type = S1[i].sensor_type;
		S[k].nr_operations = S1[i].nr_operations;
		// mai aloc zona de memorie pentru Senzori pentru ca mai adaug Senzori1
		S[k].operations_idxs = calloc(S1[i].nr_operations + 10, sizeof(int));
		if (!S[k].operations_idxs) //  verific alocarea
			return 0;
		for (int j = 0; j < S[k].nr_operations; j++)
			S[k].operations_idxs[j] = S1[i].operations_idxs[j];
		k++;
	}
	nr_senzori = k;
	scanf("%s", string); // e deja adresa (pointer) -> sir de caractere
	while (strcmp(string, "exit") != 0) {
		if (strcmp(string, "print") == 0) {
			scanf("%d", &t); // print index -> format
			if (t < 0 || t >= k) //  afara din vector
				printf("Index not in range!\n");
			else {
				//  afisare in fct de tipul de senzor
				//  PMU == 1 (sensor_type)
				if (S[t].sensor_type == PMU)
					print(S[t].sensor_data, 1);
				else
				//  TIRE == 0 (sensor_type)
					print(S[t].sensor_data, 0);
			}
		} else {
			if (strcmp(string, "analyze") == 0) {
				scanf("%d", &t); //  analyze index -> format
				if (t < 0 || t >= k)
					printf("Index not in range!\n"); //  afara din vector
				else {
					//  pentru fiecare operatie apelez fct analyze
					for (int j = 0; j < S[t].nr_operations; j++)
						analyze(S[t].sensor_data, S[t].operations_idxs[j], op);
				}
			} else {
				if (strcmp(string, "clear") == 0) {
					for (int i = 0; i < nr_senzori; i++) {
						//  verific daca senzorul trebuie sters
						int ok = check(S[i].sensor_data, S[i].sensor_type);
						if (ok == 0) {
							// senzorul trebuie sters => dezaloc mem ocupata
							free(S[i].sensor_data);
							free(S[i].operations_idxs);
							// modific vectorul de senzori
							for (int j = i; j < nr_senzori - 1; j++) {
								S[j] = S[j + 1];
							}
							nr_senzori--;
							i--;
							S = realloc(S, nr_senzori * sizeof(sensor));
							if (!S)
								return 0;
						}
					}
				}
			}
		}
		scanf("%s", string); // continui sa citesc pana intalnesc exit
	}
	for (int i = 0; i < k1; i++)
		free(S1[i].operations_idxs); // dezaloc ce am ocupat pt Senzori1
	free(S1); // free la mem ocupata de Senzori1
	for (int i = 0; i < nr_senzori; i++) {
		//  nu fac alocare separata pentru Senzori si Senzori1
		free(S[i].sensor_data);
		//  dezaloc ce am ocupat initial pt vectorul de operatii pt Senzori[i]
		// ++ ce am adaugat cand am concatenat cei doi vectori
		free(S[i].operations_idxs);
	}
	free(S); //  free la mem ocupata de Senzori
	fclose(read);  //  inchid fisierul binar
	return 0;
}
