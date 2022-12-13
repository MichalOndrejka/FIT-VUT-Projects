#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#define true 1
#define false 0
#define MMAP(pointer) {(pointer) = mmap(NULL, sizeof(*(pointer)), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);}
#define randsleep(max_time) {if (max_time != 0) usleep(rand() % (max_time+1)*1000);}
#define UNMAP(pointer) {munmap(pointer, sizeof(pointer));}

//define file, shared variables and semaphores
FILE *pfile; //output file

int *info_index; //used for printing information indices
int *molecule_index; //used for printing molecules indices
int *enough_atoms; //flag which tells processes that there is enough atoms for molecule

sem_t *index_semafor;
sem_t *oxygen_start_semafor;
sem_t *oxygen_end_semafor;
sem_t *hydrogen_start_semafor;
sem_t *hydrogen_created_semafor;
sem_t *hydrogen_end_semafor;
sem_t *molecule_semafor;
sem_t *atoms_semafor;

//initialize all variables
int init()
{
    pfile = fopen("proj2.out", "w");
    if (pfile == NULL) return 1;

    MMAP(info_index);
    *info_index = 1;
    MMAP(molecule_index);
    *molecule_index = 1;
    MMAP(enough_atoms);
    *enough_atoms = true;

    MMAP(index_semafor);
    sem_init(index_semafor, 1, 1);
    MMAP(oxygen_start_semafor);
    sem_init(oxygen_start_semafor, 1, 0);
    MMAP(oxygen_end_semafor);
    sem_init(oxygen_end_semafor, 1, 0);
    MMAP(hydrogen_start_semafor);
    sem_init(hydrogen_start_semafor, 1, 0);
    MMAP(hydrogen_end_semafor);
    sem_init(hydrogen_end_semafor, 1, 0);
    MMAP(molecule_semafor);
    sem_init(molecule_semafor, 1, 0);
    MMAP(hydrogen_created_semafor);
    sem_init(hydrogen_created_semafor, 1, 0);
    MMAP(atoms_semafor);
    sem_init(atoms_semafor, 1, 0);

    return 0;
}

//cleanup all variables
void cleanup()
{
    if (pfile != NULL) fclose(pfile);

    UNMAP(info_index);
    UNMAP(molecule_index);
    UNMAP(enough_atoms);

    UNMAP(index_semafor);
    sem_destroy(index_semafor);
    UNMAP(oxygen_start_semafor);
    sem_destroy(oxygen_start_semafor);
    UNMAP(oxygen_end_semafor);
    sem_destroy(oxygen_end_semafor);
    UNMAP(hydrogen_start_semafor);
    sem_destroy(hydrogen_start_semafor);
    UNMAP(hydrogen_end_semafor);
    sem_destroy(hydrogen_end_semafor);
    UNMAP(molecule_semafor);
    sem_destroy(molecule_semafor);
    UNMAP(hydrogen_created_semafor);
    sem_destroy(hydrogen_created_semafor);
    UNMAP(atoms_semafor);
    sem_destroy(atoms_semafor);
}

//print string using index_semafor and increment info_index
void myprint_normal(char string[], int id)
{
    sem_wait(index_semafor);
    fprintf(pfile, string, *info_index, id);
    fflush(pfile);
    (*info_index)++;
    sem_post(index_semafor);
}

//print string using index_semafor ,increment info_index and increment molecule_index
void myprint_molecule(char string[], int id)
{
    sem_wait(index_semafor);
    fprintf(pfile, string, *info_index, id, *molecule_index);
    fflush(pfile);
    (*info_index)++;
    sem_post(index_semafor);
}

void oxygenProcess(int id, int TI, int TB)
{
    myprint_normal("%d: O %d: started\n", id);

    randsleep(TI); //wait for oxygen to be created

    myprint_normal("%d: O %d: going to queue\n", id); //after oxygen is created it goes to queue
    
    sem_wait(oxygen_start_semafor); //wait for main process to allow oxygen to create molecule

    if (*enough_atoms == false) //if there are no more atoms to create molecule the process exits
    {
        myprint_normal("%d: O %d: not enough H\n", id);
        sem_post(oxygen_end_semafor); //tells main process that the process finished
        exit(0);
    }

    myprint_molecule("%d: O %d: creating molecule %d\n", id);

    sem_wait(hydrogen_created_semafor); //waits for hydrogen to print creating molecule
    sem_wait(hydrogen_created_semafor);

    randsleep(TB); //wait for molecule to be created

    sem_post(molecule_semafor); //tells hydrogen that molecule has been created
    sem_post(molecule_semafor);

    myprint_molecule("%d: O %d: molecule %d created\n", id); //molecule was officialy created

    sem_post(oxygen_end_semafor); //tells main process that the process finished

    exit(0);
}

void hydrogenProcess(int id, int TI)
{

    myprint_normal("%d: H %d: started\n", id);

    randsleep(TI); //wait for hydrogen to be created

    myprint_normal("%d: H %d: going to queue\n", id); //after hydrogen is created it goes to queue

    sem_wait(hydrogen_start_semafor); //wait for main process to allow hydrogen to create molecule

    if (*enough_atoms == false) //if there are no more atoms to create molecule the process exits
    {
        myprint_normal("%d: H %d: not enough O or H\n", id);
        sem_post(hydrogen_end_semafor); //tells main process that the process finished
        exit(0);
    }

    myprint_molecule("%d: H %d: creating molecule %d\n", id);

    sem_post(hydrogen_created_semafor); //tells oxygen that it printed creating molecule

    sem_wait(molecule_semafor); //waits for molecule to be created
    
    myprint_molecule("%d: H %d: molecule %d created\n", id);

    sem_post(hydrogen_end_semafor); //tells main process that the process finished

    exit(0);
}

//gets maximum molecules count
int getMoleculesCount(int NO, int NH)
{
    NH = NH / 2;
    return NO < NH ? NO : NH;
}

int main(int argc, char *argv[])
{
    if (argc != 5) //check for right number of argv arguments
    {
        fprintf(stderr, "Invalid number of parameters\n");
        return 1;
    }

    //save NO,NH,TI,TB from argv arguments
    int NO = atoi(argv[1]);
    int NH = atoi(argv[2]);
    int TI = atoi(argv[3]);
    int TB = atoi(argv[4]);
    //checks for invalid values
    if (NO < 0)
    {
        fprintf(stderr, "Invalid NO\n");
        return 1;
    }
    if (NH < 0)
    {
        fprintf(stderr, "Invalid NH\n");
        return 1;
    }
    if (TI < 0 || 1000 < TI)
    {
        fprintf(stderr, "Invalid TI\n");
        return 1;
    }
    if (TB < 0 || 1000 < TB)
    {
        fprintf(stderr, "Invalid TB\n");
        return 1;
    }

    //initialize file, variables and semaphores
    if (init() == 1)
    {
        fprintf(stderr, "Couldn't open file");
        return 1;
    }

    int moleculesCount = getMoleculesCount(NO, NH);

    //start NO numbers of child processes / starts creating oxygens
    for (int id = 1; id <= NO; id++)
    {
        pid_t oxygen_child = fork();
        if (oxygen_child == 0)
        {
            oxygenProcess(id, TI, TB);
        }
    }

    //start NH numbers of child processes / starts creating hydrogens
    for (int id = 1; id <= NH; id++)
    {
        pid_t hydrogen_child = fork();
        if (hydrogen_child == 0)
        {
            hydrogenProcess(id, TI);
        }
    }

    //if there is more then 0 potential molecules, it starts creating
    if  (moleculesCount > 0)
    {
        for (int i = 0; i < moleculesCount; i++)
        {
            sem_post(oxygen_start_semafor); //allows one oxygen and two hydrogens to form a molecule
            sem_post(hydrogen_start_semafor);
            sem_post(hydrogen_start_semafor);

            sem_wait(oxygen_end_semafor); //waits for one oxygen and two hydrogens that molecule has been created
            sem_wait(hydrogen_end_semafor);
            sem_wait(hydrogen_end_semafor);
            
            (*molecule_index)++; //increase molecule index
        }
    }

    //after all potential molecules has been created this flag is changed to false
    *enough_atoms = false;

    //all processes which cant form a molecule anymore are cleaned
    for (int i = 0; i < NO - moleculesCount; i++) 
    {
        sem_post(oxygen_start_semafor);
        sem_wait(oxygen_end_semafor);
    }
    for (int i = 0; i < NH - moleculesCount*2; i++) 
    {
        sem_post(hydrogen_start_semafor);
        sem_wait(hydrogen_end_semafor);
    }
    
    //close file, and unmap variables and semaphore
    cleanup();

    return 0;
}
