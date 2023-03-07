#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <iomanip>
//#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
//#include<pthread.h>
//#include<unistd.h>
//#include<semaphore.h>
using namespace std;


//bankers algo

struct process
{
    char name;
    int AT, BT, WT, TAT, RT, CT;

}
Q1[10], Q2[10], Q3[10];   /*Three queues*/

void sortByArrival(int n)
{
    struct process temp;
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (Q1[i].AT > Q1[j].AT)
            {
                temp = Q1[i];
                Q1[i] = Q1[j];
                Q1[j] = temp;
            }
        }
    }
}
struct process2
{
    char name;
    int at, bt, ct, wt, tt;       //at=arival time,bt=burst time,ct=completion time, wt=waiting time ,tt=turnaround time
    int complete;
}
p[10];

void sortByArrival2(int m)
{
    struct process2 temp;
    int i, j;

    for (i = 0; i < m - 1; i++)
    {                           	// Selection Sort applied 
        for (j = i + 1; j < m; j++)
        {

            if (p[i].at > p[j].at)
            {

                temp = p[i];                       	// Swaping of earlier process to front 
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

int allocation[10][3], need[10][3], Max[10][3], available[10][3];
int proc;
int current[3];
bool executed[10], come;
void IMP()
{
    come = false;
    for (int i = 0; i < 10; ++i)
    {
        executed[i] = false;
    }
}

void Calculate()
{
    IMP();
    int i, j;
    for (i = 0; i < proc; ++i)
    {
        for (j = 0; j < proc; ++j)
        {
            while (executed[j] && j < proc - 1)
            {
                j++;
            }
            if (need[j][0] <= current[0] && need[j][1] <= current[1] && need[j][2] <= current[2])
            {
                if (!executed[j])
                {
                    executed[j] = true;
                    current[0] += allocation[j][0]; current[1] += allocation[j][1]; current[2] += allocation[j][2];
                    cout << "\nProcess P" << j + 1;
                    cout << "\nCurrent: " << current[0] << " " << current[1] << " " << current[2] << "\n";
                    cout << "\nProcess executed without deadlock";
                    come = true;
                    break;
                }
            }
        }
        if (!come)
        {
            cout << "\n  Dead lock\n\n";
            break;
        }
        else
        {
            come = false;
        }
    }
}


//inverted-table

struct table {
    int page_no;
    int process_id;
};

void invert_table()
{

    int no_of_frames;
    cout << "insert the number of frames in the memory:" << endl;
    cin >> no_of_frames;
    table* invertedpagetable = new table[no_of_frames];

    for (int i = 0; i < no_of_frames; i++)
    {
        cout << "insert the page number and process id:";
        cin >> invertedpagetable[i].page_no;
        cin >> invertedpagetable[i].process_id;
        cout << endl;
    }


    for (int i = 0; i < no_of_frames; i++)
    {
        cout << "   " << i << "|" << invertedpagetable[i].page_no << "|" << invertedpagetable[i].process_id << endl;



    }

    int pageno, processno;
    cout << "Enter the page no which cpu wants to search:";
    cin >> pageno;
    cout << "Enter the process no for which cpu wants to search the page:";
    cin >> processno;

    bool found = false;

    for (int i = 0; i < no_of_frames; i++)
    {
        if (invertedpagetable[i].page_no == pageno && invertedpagetable[i].process_id == processno) {
            cout << "page is located at frame no:" << i;
            found = true;
        }

    }

    if (!found)
        cout << "page number doesn't exist for the given page number";

}


//hashmap


const int TABLE_SIZE = 64;

class HashNode
{
public:
    int page_no;
    int frame_no;
    HashNode* next;
    HashNode(int page_no, int frame_no)
    {
        this->page_no = page_no;
        this->frame_no = frame_no;
        this->next = NULL;
    }
};


class HashMap
{
private:
    HashNode** htable;
public:
    HashMap()
    {
        htable = new HashNode * [TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++)
            htable[i] = NULL;
    }
    ~HashMap()
    {
        for (int i = 0; i < TABLE_SIZE; ++i)
        {
            HashNode* entry = htable[i];
            while (entry != NULL)
            {
                HashNode* prev = entry;
                entry = entry->next;
                delete prev;
            }
        }
        delete[] htable;
    }

    int HashFunc(int page_no)
    {
        return page_no % TABLE_SIZE;
    }


    void Insert(int page_no, int frame_no)
    {
        int hash_val = HashFunc(page_no);
        HashNode* prev = NULL;
        HashNode* entry = htable[hash_val];
        while (entry != NULL)
        {
            prev = entry;
            entry = entry->next;
        }
        if (entry == NULL)
        {
            entry = new HashNode(page_no, frame_no);
            if (prev == NULL)
            {
                htable[hash_val] = entry;
            }
            else
            {
                prev->next = entry;
            }
        }
        else
        {
            entry->frame_no = frame_no;
        }
    }

    void Remove(int page_no)
    {
        int hash_val = HashFunc(page_no);
        HashNode* entry = htable[hash_val];
        HashNode* prev = NULL;
        if (entry == NULL || entry->page_no != page_no)
        {
            cout << "No value found at page_no " << page_no << endl;
            return;
        }
        while (entry->next != NULL)
        {
            prev = entry;
            entry = entry->next;
        }
        if (prev != NULL)
        {
            prev->next = entry->next;
        }
        delete entry;
        cout << "Element Deleted" << endl;
    }

    int Search(int page_no)
    {
        bool flag = false;
        int hash_val = HashFunc(page_no);
        HashNode* entry = htable[hash_val];
        while (entry != NULL)
        {
            if (entry->page_no == page_no)
            {
                cout << "page is mapped at frame number:" << entry->frame_no << " ";
                flag = true;
            }
            entry = entry->next;
        }
        if (!flag)
            return -1;
    }
};

void hashmap()
{
    HashMap hash;
    int page_no, frame_no;
    int choice;
    while (1)
    {
        cout << "\n----------------------" << endl;
        cout << "Operations on Hashed Page Table" << endl;
        cout << "\n----------------------" << endl;
        cout << "1.Insert element into the table" << endl;
        cout << "2.Search element from the page_no" << endl;
        cout << "3.Delete element at a page_no" << endl;
        cout << "4.Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "Enter frame to be inserted: ";
            cin >> frame_no;
            cout << "Enter page_no at which frame no to be inserted: ";
            cin >> page_no;
            hash.Insert(page_no, frame_no);
            break;
        case 2:
            cout << "Enter page_no of the frameno to be searched to be searched: ";
            cin >> page_no;
            cout << "frame at page_no " << page_no << " : ";
            if (hash.Search(page_no) == -1)
            {
                cout << "No element found at page_no " << page_no << endl;
                continue;
            }
            break;
        case 3:
            cout << "Enter page_no of the element to be deleted: ";
            cin >> page_no;
            hash.Remove(page_no);
            break;
        case 4:
            exit(1);
        default:
            cout << "\nEnter correct option\n";
        }
    }

}


//second-chance


int ReplacePage(int page, int* frame_arr, int* reference_bit, int frames, int i)
{
    while (true)
    {

        if (reference_bit[i] == 0)
        {
            frame_arr[i] = page;
            return (i + 1) % frames;
        }
        reference_bit[i] = 0;
        i = (i + 1) % frames;
    }
}

void PageFaults(int* reference_pages, float frames, int pageno)
{
    int pointer = 0, page = 0;
    float faults = 0;
    int* frame_arr = new int(frames);


    for (int i = 0; i < frames; i++) {
        frame_arr[i] = 0;
    }


    int* reference_bit = new int(frames);

    int i;
    for (i = 0; i < pageno; i++)
    {
        page = reference_pages[i];

        int j;

        bool secondchance = false;
        for (j = 0; j < frames; j++)
        {

            if (frame_arr[j] == page)
            {
                reference_bit[j] = 1;
                secondchance = true;
            }

        }


        if (secondchance == false)
        {
            pointer = ReplacePage(page, frame_arr, reference_bit, frames, pointer);
            faults++;
        }
    }
    float newfault;
    newfault = faults + 1;
    if (pageno - newfault < 0) {
        newfault = faults;
    }
    cout << "                     Page Hits: " << pageno - newfault << endl;
    cout << "                     Page Faults: " << newfault << endl;
    float probab;
    probab = (newfault) / pageno;
    cout << "                     Prob of Fault: " << probab << endl;
    float percentage;
    percentage = (probab) * 100;
    cout << "                     Fault Percentage: " << percentage << "%";
    cout << endl << endl;
}

void SecondChanceAlgo() {

    int frames;
    float refno;
    cout << "Enter total no of reference pages: ";
    cin >> refno;
    while (refno <= 0) {
        cout << " No. of Reference Pages should be greater than 0." << endl;
        cout << "Enter no of frames: ";
        cin >> refno;
    }
    int* reference_pages = new int[refno];
    for (int i = 0; i < refno; i++) {
        cout << setw(2) << i + 1 << ": ";
        cin >> reference_pages[i];
        while (reference_pages[i] < 0) {
            cout << " Reference Pages should be greater than -1" << endl;
            cout << setw(2) << i << ": ";
            cin >> reference_pages[i];
        }
    }
    cout << "Enter no of frames: ";
    cin >> frames;
    while (frames <= 0) {
        cout << " No. of Frames should be greater than 0." << endl;
        cout << "Enter no of frames: ";
        cin >> frames;
    }

    PageFaults(reference_pages, frames, refno);

}

//semaphores

/*
sem_t semaphore1; //1st semaphore
sem_t semaphore2; //1st semaphore

int student_id = 1;
int student_number;

//thread for TA
void* ta(void* args)
{
    while (true)
    {
        sem_wait(&semaphore1);
        if (student_number == 0) {
            cout << "TA doing his work...\n";
            break;
        }
        else {

        }
        sem_post(&semaphore1);
        usleep(5000);
    }
    return NULL;
}

//thread for students
void* student(void* args)
{
    sem_wait(&semaphore1);
    cout << "Student#" << student_id << " giving evaluation." << endl;
    student_id++;
    student_number--;
    sem_post(&semaphore1);
    return NULL;
}


void semaphoresfunc()
{
    cout << "Enter the number of students:";  //taking number of students from the user
    int no_of_students;
    cin >> no_of_students;
    student_number = no_of_students;
    int total_threads = no_of_students + 1; //total thread required

    pthread_t a_thread[total_threads];

    sem_init(&semaphore1, 0, 1);
    sem_init(&semaphore2, 0, 1);

    //thread for TA
    pthread_create(&a_thread[0], NULL, ta, NULL);

    //threads for students
    for (int i = 1; i <= no_of_students; i++)
    {
        pthread_create(&a_thread[i], NULL, student, NULL);
    }

    //joining thread
    for (int i = 0; i < total_threads; i++)
    {
        pthread_join(a_thread[i], NULL);
    }

    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);
}

*/


void memory_manage() {

    int mem_choice;

    cout << "Choose from following: " << endl << endl;
    cout << "1. Inverted Table" << endl;
    cout << "2. Hash Map Table" << endl;
    cout << "Choice: ";
    cin >> mem_choice;
    while (mem_choice < 1 || mem_choice>2) {
        cout << " Choose valid option." << endl;
        cin >> mem_choice;
    }
    system("CLS");
    if (mem_choice == 1) {
        cout << endl;
        cout << " **** 1.Inverted Table ****" << endl << endl;
        invert_table();
    }
    else if (mem_choice == 2) {
        cout << endl;
        cout << " **** 2.Hash Map Table ****" << endl << endl;
        hashmap();
    }
}

int main()
{

    cout << endl << endl << endl << endl << endl;

    cout <<"                       |        OS PROJECT          |         " << endl;
    cout <<"                       |            BY              |        "  << endl;
    cout <<"                       |    Iqbal Elahi 20L-0908    |                   " << endl;
    cout <<"                       |    Shaheer Azam 20L-1161   |                    " << endl;
    cout <<"                       |     Ibrar Ali 20L-2125     |                  " << endl;


    cout << endl << endl << endl;

    int enter;
    cout << "                         Continue or Exit (1/2) : "; 
    cin >> enter;
   
    if (enter == 1) {
        system("CLS");
    }
    else {
        return 0;
    }
    int choice, cpu_choice;
    cout << endl << endl << endl;
    cout << "                       |        MENU          |         " << endl;
    cout << "                     Press 1 for CPU Scheduling " << endl;
    cout << "                     Press 2 for Semephore " << endl;
    cout << "                     Press 3 for Deadlock Solution(Bankers Algorithm) " << endl;
    cout << "                     Press 4 for Memory Management " << endl;
    cout << "                     Press 5 for Page Replacement " << endl;
    cout << endl << endl;
    cout << "                     Enter Choice:  ";
    cin >> choice;
    while (choice < 1 || choice > 5) {
        cout << "                     Enter Valid Choice." << endl;
        cout << "                     Enter Choice:  ";
        cin >> choice;
    }
    system("CLS");
    if (choice == 1)
    {
        cout << endl;
        cout << "                       | 1.CPU Scheduling |         " << endl << endl;

        cout << "                     Press 1 for Multi Level Queue" << endl;
        cout << "                     Press 2 for Multi Level Feedback Queue" << endl;
        cout << "                     Press 3 for Longest Job First" << endl;
        cout << "                     Press 4 for Highest Feedback Ratio Next" << endl;
        cout << endl << endl;
        cout << "                     Enter Choice:  ";
        cin >> cpu_choice;
        while (cpu_choice < 1 || cpu_choice > 4) {
            cout << "                     Enter Valid Choice." << endl;
            cout << "                     Enter Choice:  ";
            cin >> cpu_choice;
        }
        system("CLS");
        if (cpu_choice == 1)
        {
            cout << endl;
            cout << "                       | 1.Multi Level Queue |         " << endl << endl;
            int p[20], bt[20], su[20], wt[20], tat[20], i, k, n, temp;
            float wtavg, tatavg;
            cout << "                     Enter the number of processes:";
            cin >> n;
            for (i = 0; i < n; i++)
            {
                p[i] = i;
                cout << "                     Enter the Burst Time of Process " << i << " : ";
                cin >> bt[i];
                cout << "                     System/User Process (0/1) ? ";
                cin >> su[i];
            }
            for (i = 0; i < n; i++)
                for (k = i + 1; k < n; k++)
                    if (su[i] > su[k])
                    {
                        temp = p[i];
                        p[i] = p[k];
                        p[k] = temp;
                        temp = bt[i];
                        bt[i] = bt[k];
                        bt[k] = temp;
                        temp = su[i];
                        su[i] = su[k];
                        su[k] = temp;
                    }
            wtavg = wt[0] = 0;
            tatavg = tat[0] = bt[0];
            for (i = 1; i < n; i++)
            {
                wt[i] = wt[i - 1] + bt[i - 1];
                tat[i] = tat[i - 1] + bt[i];
                wtavg = wtavg + wt[i];
                tatavg = tatavg + tat[i];
            }
            cout << "\nPROCESS\t\t SYSTEM/USER PROCESS \tBURST TIME\tWAITING TIME\tTURNAROUND TIME";
            for (i = 0; i < n; i++)
                cout << "\n" << p[i] << "\t\t\t" << su[i] << "\t\t" << bt[i] << "\t\t" << wt[i] << "\t\t" << tat[i];
            cout << "\nAverage Waiting Time is : " << wtavg / n;
            cout << "\nAverage Turnaround Time is : " << tatavg / n;
            cout << "\n";

        }
        else if (cpu_choice == 2)
        {
            cout << endl;
            cout << "                     | 2.Multi Level Feedback Queue |         " << endl << endl;
            int i, n, j, k = 0, r = 0, time = 0, tq1 = 17, tq2 = 25, flag = 0;
            char c;
            cout << "                     Enter no of processes:";
            cin >> n;
            for (i = 0, c = 'A'; i < n; i++, c++)
            {
                Q1[i].name = c;
                cout << "\n                     Enter the arrival time of process " << Q1[i].name << " : ";
                cin >> Q1[i].AT;
                cout << "\n                     Enter the burst time of process " << Q1[i].name << " : ";
                cin >> Q1[i].BT;
                Q1[i].RT = Q1[i].BT;/*save burst time in remaining time for each process*/

            }
            sortByArrival(n);
            time = Q1[0].AT;
            cout << "                     Process in first queue following RR with qt=17";
            cout << "\nProcess\t\tRT\t\tWT\t\tTAT\t\t";
            for (i = 0; i < n; i++)
            {

                if (Q1[i].RT <= tq1)
                {

                    time += Q1[i].RT;
                    Q1[i].RT = 0;
                    Q1[i].WT = time - Q1[i].AT - Q1[i].BT;
                    Q1[i].TAT = time - Q1[i].AT;
                    cout << "\n" << Q1[i].name << "\t\t" << Q1[i].BT << "\t\t" << Q1[i].WT << "\t\t" << Q1[i].TAT;

                }
                else/*process moves to queue 2 with qt=25*/
                {
                    Q2[k].WT = time;
                    time += tq1;
                    Q1[i].RT -= tq1;
                    Q2[k].BT = Q1[i].RT;
                    Q2[k].RT = Q2[k].BT;
                    Q2[k].name = Q1[i].name;
                    k = k + 1;
                    flag = 1;   
                }
            }
            if (flag == 1)
            {
                cout << "\n                     Process in second queue following RR with qt=25";
                cout << "\nProcess\t\tRT\t\tWT\t\tTAT\t\t";
            }
            for (i = 0; i < k; i++)
            {
                if (Q2[i].RT <= tq2)
                {
                    time += Q2[i].RT;
                    Q2[i].RT = 0;
                    Q2[i].WT = time - tq1 - Q2[i].BT;
                    Q2[i].TAT = time - Q2[i].AT;
                    cout << "\n" << Q2[i].name << "\t\t" << Q2[i].BT << "\t\t" << Q2[i].WT << "\t\t" << Q2[i].TAT;

                }
                else/*process moves to queue 3 with FCFS*/
                {
                    Q3[r].AT = time;
                    time += tq2;
                    Q2[i].RT -= tq2;
                    Q3[r].BT = Q2[i].RT;
                    Q3[r].RT = Q3[r].BT;
                    Q3[r].name = Q2[i].name;
                    r = r + 1;
                    flag = 2;
                }
            }

            {
                if (flag == 2)
                    cout << "\n                     Process in third queue following FCFS ";
            }
            for (i = 0; i < r; i++)
            {
                if (i == 0)
                    Q3[i].CT = Q3[i].BT + time - tq1 - tq2;
                else
                    Q3[i].CT = Q3[i - 1].CT + Q3[i].BT;

            }

            for (i = 0; i < r; i++)
            {
                Q3[i].TAT = Q3[i].CT;
                Q3[i].WT = Q3[i].TAT - Q3[i].BT;
                cout << "\n" << Q3[i].name << "\t\t" << Q3[i].BT << "\t\t" << Q3[i].WT << "\t\t" << Q3[i].TAT;
                cout << "\n";

            }

            cout << endl << endl << endl;
        }

        else if (cpu_choice == 3)
        {
            cout << endl;
            cout << "                     | 3.Longest Job First |         " << endl << endl;
            int i, n, j, min, p[10] = { 1,2,3,4,5,6,7,8,9,10 };
            int B_T[1000], A_V_T[1000], W_T[1000], temp[20], temp2, temp1, T_A_T[10];
            int bt = 0, ta = 0, sum = 0, k = 1;
            float AVR_W_T = 0, AVR_T_A_T = 0, T_A_T_SUM = 0, W_T_SUM = 0;
            cout << "                     Enter the Number of processes : ";
            cin >> n;
            for (i = 0; i < n; i++)
            {
                cout << "\n                     Enter The Arrival Time Of Process " << i + 1 << " : ";
                cin >> A_V_T[i];
                cout << "                     Enter The Burst Time Of Process%d " << i + 1 << " : ";
                cin >> B_T[i];

            }
            for (i = 0; i < n; i++)                     //sorting processes on the bases of arrival time
            {
                for (j = 0; j < n; j++)
                {
                    if (A_V_T[i] < A_V_T[j])
                    {
                        temp2 = p[j];
                        p[j] = p[i];
                        p[i] = temp2;
                        temp2 = A_V_T[j];
                        A_V_T[j] = A_V_T[i];
                        A_V_T[i] = temp2;
                        temp2 = B_T[j];
                        B_T[j] = B_T[i];
                        B_T[i] = temp2;
                    }
                }
            }

            for (j = 0; j < n; j++)
            {
                bt = bt + B_T[j];
                min = B_T[k];
                for (i = k; i < n; i++)
                {
                    if (bt >= A_V_T[i] && B_T[i] > min)
                    {
                        temp2 = p[k];
                        p[k] = p[i];
                        p[i] = temp2;
                        temp2 = A_V_T[k];
                        A_V_T[k] = A_V_T[i];
                        A_V_T[i] = temp2;
                        temp2 = B_T[k];
                        B_T[k] = B_T[i];
                        B_T[i] = temp2;
                    }
                }
                k++;
            }
            W_T[0] = 0;
            for (i = 1; i < n; i++)
            {
                sum = sum + B_T[i - 1];
                W_T[i] = sum - A_V_T[i];
                W_T_SUM = W_T_SUM + W_T[i];
            }

            AVR_W_T = (W_T_SUM / n);
            for (i = 0; i < n; i++)
            {
                ta = ta + B_T[i];
                T_A_T[i] = ta - A_V_T[i];
                T_A_T_SUM = T_A_T_SUM + T_A_T[i];
            }

            AVR_T_A_T = (T_A_T_SUM / n);
            cout << "\n\n";
            for (i = 1; i < n; i++)
            {
                for (j = 1; j < n; j++)
                {
                    if (B_T[j] < B_T[j + 1])
                    {
                        temp1 = B_T[j];
                        B_T[j] = B_T[j + 1];
                        B_T[j + 1] = temp1;
                    }
                }
            }
            temp[0] = 0;
            cout << "\nProcess Name\tBurst Time\t Arrival Time\t Waiting Time\t Turn Around Time";
            for (i = 0; i < n; i++)
            {

                cout << "\np" << p[i] << "\t\t" << B_T[i] << "\t\t" << A_V_T[i] << "\t\t" << W_T[i] << "\t\t" << T_A_T[i];
            }
            cout << "\n\nAVERAGE WAITING TIME : " << AVR_W_T;
            cout << "\nAVERAGE TURN AROUND TIME : " << AVR_T_A_T;
            cout << "\n";

            cout << endl << endl;
        }
        else if (cpu_choice == 4)
        {
            cout << endl;
            cout << "                     | 4.Highest Feedback Ratio Next |         " << endl << endl;
            int i, m, j, t, sum_bt = 0;
            char c;
            float avgwt = 0, avgtt = 0;
            cout << "                     Enter number of process:";
            cin >> m;
            int arriv[20];
            for (i = 0; i < m; i++)
            {
                cout << "\n                     Enter Arrival Time:";
                cout << "p" << i + 1 << " : ";
                cin >> arriv[i];
            }

            int burst[20];
            for (i = 0; i < m; i++)
            {
                cout << "\n                     Enter Burst Time:";
                cout << "p" << i + 1 << " : ";
                cin >> burst[i];
            }
            for (i = 0, c = 'A'; i < m; i++, c++)
            {
                p[i].name = c;
                p[i].at = arriv[i];
                p[i].bt = burst[i];

                p[i].complete = 0;

                sum_bt += p[i].bt;                  // the Variable for the sum of all Burst Times 
            }

            sortByArrival2(m);                //sorting by arrival time
            cout << "\nName\tArrival Time\tBurst Time\tWaiting Time";
            cout << "\tTurnAround Time";
            for (t = p[0].at; t < sum_bt;)
            {


                float hrr = -1111;     //setting response ration to the lowest
                float temp;

                int loc;
                for (i = 0; i < m; i++)
                {

                    if (p[i].at <= t && p[i].complete != 1)
                    {
                        temp = (((t - p[i].at) + p[i].bt) / p[i].bt);          // Calculating the Response Ratio 

                        if (hrr < temp)
                        {

                            hrr = temp;

                            loc = i;
                        }
                    }
                }

                t += p[loc].bt;

                p[loc].wt = t - p[loc].at - p[loc].bt;     //Waiting time calculation

                p[loc].tt = t - p[loc].at;             //turn around time calculation

                avgtt += p[loc].tt;


                // Updating  the Completion Status 
                p[loc].complete = 1;

                avgwt += p[loc].wt;
                cout << "\n" << p[loc].name << "\t\t" << p[loc].at << "\t\t";
                cout << p[loc].bt << "\t\t" << p[loc].wt << "\t\t";;
                cout << p[loc].tt;
            }
            cout << "\n                     The Average waiting time: " << avgwt / m << endl;
            cout << "                     The Average Turn Around time: " << avgtt / m << endl;

            cout << endl << endl;
        }

    }
    else if (choice == 2) {
        //semaphoresfunc();
    }

    
    else if (choice == 3)
    {
        cout << endl;
        cout << "                     | 3.Banker's Algorithm |         " << endl << endl;
        int keepon = 1;
        cout << "                     Enter No. of processes: ";
        cin >> proc;
        cout << "\n";
        cout << "                     Enter the current resources: " << endl;
        cout << "                     ";
        cin >> current[0];
        cout << "                     "; 
        cin >> current[1];
        cout << "                     "; 
        cin>> current[2];
        for (int i = 0; i < proc; ++i)
        {
            cout << "\n\n                     	Process P" << i + 1 << " Details\n";
            cout << "                     Enter Allocation : " << endl;
            cout << "                     ";
            cin >> allocation[i][0];
            cout << "                     "; 
            cin >> allocation[i][1];
            cout << "                     "; 
            cin>> allocation[i][2];
            cout << "                     Enter Max :" << endl;
            cout << "                     ";
            cin >> Max[i][0];
            cout << "                     ";
            cin >> Max[i][1];
            cout << "                     ";
            cin >> Max[i][2];
            need[i][0] = Max[i][0] - allocation[i][0];
            need[i][1] = Max[i][1] - allocation[i][1];
            need[i][2] = Max[i][2] - allocation[i][2];
        }
        cout << "\n\n                     	 Table for Bankers Algo\n\n";
        cout << "Initial Resources: " << current[0] << " " << current[1] << " " << current[2] << "\n\n";
        cout << "Process    Max    Allocation    Need\n";
        for (int i = 0; i < proc; ++i)
        {
            cout << "  P" << i + 1 << "    ";
            cout << "  " << Max[i][0] << " " << Max[i][1] << " " << Max[i][2] << "     ";
            cout << " " << allocation[i][0] << " " << allocation[i][1] << " " << allocation[i][2] << "     ";
            cout << " " << need[i][0] << " " << need[i][1] << " " << need[i][2];
            cout << "\n";
        }
        cout << "\n\n";
        Calculate();
    }

    else if (choice == 4) {
    cout << endl;
    cout << "                     | 4.Memory Management |         " << endl << endl;
    memory_manage();
}
    else if (choice == 5) {
    cout << endl;
    cout << "                     | 5.Second Chance Algorithm |         " << endl << endl;
    SecondChanceAlgo();
    
}
    return 0;
}



