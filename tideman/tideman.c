#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int indice_de(int candidato, int ranks[]);
bool verificador_ciclo(int indice_actual);
bool verificador2(int indice_actual);
bool verificador(int ganador, int perdedor);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    // Si el nombre coincide con el de un candidato válido, debo actualizar el arreglo ranks para indicar que el votante tiene a su candidato como su preferencia rank.
    // para cada rank (lugar) evaluaré cada candidato (en el loop llamaré j): en este rank, ¿el nombre coincide con el del candidato j? si sí, escribiré J en el respectivo rank
    // necesito for i in rank (0 - candidate_count), for j in candidate_count y agregar j a ranks cuando el j name en coincida:

    for (int i = 0; i < candidate_count; i++)
    {

        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            //printf("%s\n", candidates[i]);
            return true;
        }

    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    //Toma el array de preferencias que acabo de crear en la función anterior
    //Actualiza el arreglo de preferencias de dos simensiones [i][j]
    //ese arreglo representa el número de personas que prefieren al candidato i sobre el j.
    // si son tres candidatos A,B y C; entonces el arreglo será de nueve.
    // Quiere decir que para cada arreglo tendré que registrar 9 veces +0 o +1 votos.
    //Para preferencia A: ¿prefirió a sobre b? no, entonces 0;
    // prefirio A sobre b? sí, entonces 1; y así
    //¿Como verifico esto? paso sobre cada índice del array, selecciono el candidato que hay allí y pregunto:
    //¿Está este candidato ubicado (menor indice en ranks) antes que los demás candidatos?
    // si no, +0 en el índice de preferences candidato vs candidato.
    // si sí, +1 en el índice de preferencias candidato vs candidato

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (ranks[i] == j)
            {
                for (int k = 0; k < candidate_count; k++)
                {
                    if (i < indice_de(k, ranks))
                    {
                        preferences[j][k]++;
                        //printf("%i ", preferences[j][k]);
                    }
                    else
                    {
                        preferences[j][k] += 0;
                        //printf("%i ", preferences[j][k]);
                    }
                }
            }
        }
    }
    return;
}
// FUNCIÓN PARA IDENTIFICAR EL ÍNDICE EN RANKS DE ALGÚN CANDIDATO.
int indice_de(int candidato, int ranks[])
{
    int indice = 100;
    for (int i = 0; i < candidate_count; i++)
    {
        if (ranks[i] == candidato)
        {
            indice = i;
        }
    }
    return indice;
}
// Record pairs of candidates where one is preferred over the other
// Primero empezaré con los índices para ubicar en pair el resultado.
// Luego analizaré el array 2d de la siguiente manera: el primer índice A será el 0 y el último será el menos 1.
// El primer índice B será el +1 del anterior haste el último.
// Si el A - B es mayor que el B - A el ganador es A.
// Si el B - A es mayor que el A - B, el ganador es B.
// el anterior resultado se ubica en el índice inicial para los pairs.
void add_pairs(void)
{
    // TODO

    //while(true)
    //{

    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int k = i + 1; k < candidate_count; k++)
        {
            if (preferences[i][k] > preferences[k][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = k;
                pair_count++;
            }
            else if (preferences[i][k] < preferences[k][i])
            {
                pairs[pair_count].winner = k;
                pairs[pair_count].loser = i;
                pair_count++;
            }

        }

    }
    //for (int m = 0; m < ind; m ++)
    //      {
    // printf("\n\nwinner: %i\nloser: %i\n", pairs[m].winner, pairs[m].loser);
    //    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // Voy a crear una función similar a la anterior, que me pemita crear un array de diferencias.
    // Voy a crear una copia del anterior array y la voy a organizar en orden descendiente.
    // voy a crear un tercer array en el que estén los índices del array ordenado.
    // voy a roganizar el array de pares con base en el anterior ordenado.

    int array_desordenado[pair_count];
    int ind = 0;
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int k = i + 1; k < candidate_count; k++)
        {
            if (preferences[i][k] > preferences[k][i])
            {
                array_desordenado[ind] = preferences[i][k] - preferences[k][i];
                //printf("diferencia: %i", array_desordenado[ind]);
                ind++;
            }
            else if (preferences[i][k] < preferences[k][i])
            {
                array_desordenado[ind] = preferences[k][i] - preferences[i][k];
                //printf("diferencia: %i", array_desordenado[ind]);
                ind++;
            }
        }
    }



    for (int id = 1; id < pair_count; id++)
    {
        int ve = array_desordenado[id];
        pair ve_pairs = pairs[id];
        int ii = id - 1;
        while (ii >= 0)
        {
            if (ve > array_desordenado[ii])
            {
                array_desordenado[ii + 1] = array_desordenado[ii];
                pairs[ii + 1] = pairs[ii];
                array_desordenado[ii] = ve;
                pairs[ii] = ve_pairs;
                ii--;
            }
            else
            {
                break;
            }

        }
    }

    //for (int i = 0; i < pair_count; i++)
    //{
    //printf("pairs ordenado ganador: %i\n perdedor: %i\n", pairs[i].winner, pairs[i].loser );
    //printf("diferencia: %i\n", array_desordenado[i] );
    //}



    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{

    for (int i = 0; i < pair_count; i++)
    {

        if (!verificador(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}



bool verificador(int ganador, int perdedor)
{
    if (ganador == perdedor)
    {
        return true;
    }


    int i = 0;

    while (i < pair_count)
    {
        if (pairs[i].winner == perdedor && locked[pairs[i].winner][pairs[i].loser] == true)
        {
            if (pairs[i].loser == ganador)
            {
                return true;
            }
            else if (verificador(ganador, pairs[i].loser))
            {
                return true;
            }
        }
        i++;
    }
    return false;
}


// Print the winner of the election
void print_winner(void)
{
    // TODO
    //buscaré en locked por columnas y sumaré los verdaderos con +1.
    //si la suma de la columna es 0, ese será el ganador.

    for (int i = 0; i < candidate_count; i++)
    {
        int conteo_ganador = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                conteo_ganador += 1;
            }
            //printf("%s\n", candidates[j]);
        }
        //printf("%i\n", conteo_ganador);
        if (conteo_ganador == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}