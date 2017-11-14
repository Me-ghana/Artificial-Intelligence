#include <iostream>       // std::cout
#include <queue>          // std::queue
#include <fstream>
#include <list>
#include <deque>
#include <math.h>
#include <time.h>
#include <string.h>

using namespace std;

struct gridNode
{
    int x;
    int y;
};

//function for printing the elements in a list
void showlist(list <gridNode> g)
{
    list <gridNode> :: iterator it;
    for(it = g.begin(); it != g.end(); ++it)
    {
        gridNode griddy = *it;
        cout << "x:" << griddy.x << " y:" << griddy.y << "   ";
    }
    cout << endl;
}

void printNursery(list<gridNode> checkList,list<gridNode> treeList, int n)
{
    int  nursery[n][n];
    memset(nursery,0,sizeof(nursery));

   int x,y;
    list <gridNode> :: iterator it;
    //has all the locations of the lizards

    int it_count = 0;
   for(it = checkList.begin(); it != checkList.end(); ++it)
    {
        if (it_count > 0)
        {
            gridNode griddy = *it;
            x = griddy.x;
            y = griddy.y;
            nursery[x][y] = 1;
        }
        it_count++;
    }
    //has all the locations of the trees   
    for(it = treeList.begin(); it != treeList.end(); ++it)
    {
        gridNode griddy = *it;
        x = griddy.x;
        y = griddy.y;
        nursery[x][y] = 2;
    }

    for (int l = 0; l < n; l++)
    {
        for (int m = 0; m < n ; m++)
        {
            cout << nursery[l][m] << " ";
        }
        cout << endl;
    }
}

//Print out result and matrix to output.txt
void fileOutputFail()
{
    ofstream myfile;
    myfile.open("output.txt");
    myfile << "FAIL" << endl;
    myfile.close();
    return;
}

void fileOutputSuccess(list<gridNode> checkList,list<gridNode> treeList, int n)
{
    ofstream myfile;
    myfile.open("output.txt");

    int  nursery[n][n];
    memset(nursery,0,sizeof(nursery));

    int x,y;
    list <gridNode> :: iterator it;
    //has all the locations of the lizards

    int it_count = 0;
    for(it = checkList.begin(); it != checkList.end(); ++it)
    {
        if (it_count > 0)
        {
            gridNode griddy = *it;
            x = griddy.x;
            y = griddy.y;
            nursery[x][y] = 1;
        }
        it_count++;
    }
    //has all the locations of the trees   
    for(it = treeList.begin(); it != treeList.end(); ++it)
    {
        gridNode griddy = *it;
        x = griddy.x;
        y = griddy.y;
        nursery[x][y] = 2;
    }

    myfile << "OK" << endl;
    for (int l = 0; l < n; l++)
    {
        for (int m = 0; m < n ; m++)
        {
            myfile << nursery[l][m];
        }
        myfile << endl;
    }
    myfile.close();
}

void fileOutputSuccess_SA(int** nursery, int n)
{
    ofstream myfile;
    myfile.open("output.txt");

    myfile << "OK" << endl;
    for (int l = 0; l < n; l++)
    {
        for (int m = 0; m < n ; m++)
        {
            myfile << nursery[l][m];
        }
        myfile << endl;
    }
    myfile.close();
}

//function for printing the elements in a queue (each first node of a list)
void showq(queue <list<gridNode> >  gq)
{
    queue <list<gridNode> > g = gq;
    while (!g.empty())
    {
        list <gridNode> mylist = g.front(); 
     //   gridNode griddy = mylist.front();
    //    cout << '\t' << griddy.y;
        showlist(mylist);
        g.pop();
        cout << endl;
    }
    cout << '\n';
}

void deshowq(deque <list<gridNode> >  gq)
{
    deque <list<gridNode> > g = gq;
    while (!g.empty())
    {
        list <gridNode> mylist = g.front(); 
     //   gridNode griddy = mylist.front();
    //    cout << '\t' << griddy.y;
        showlist(mylist);
        g.pop_front();
        cout << endl;
    }
    cout << '\n';
}

void printGrid(int n, int** grid) //print our nursery grid
{
    int i,j;
    for (i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

//Goal test checks if there are p+1 lizards because the list.size() will give us back one extra value, the node tracking the depth
//This is an IMPLICIT goal test
int GoalTest(list<gridNode> list, int p)
{
    if (list.size() == (p+1))
    {
        return 0; //0 indicates goal test true
    }
    else 
    {
        return 1; //1 indicated goal test false, or fail
    }
}


//Here, ExpandV2 passes the row with the "safe" sports indicated
//This function will create all safe combinations of how lizards can be placed here and then add it to the linked list which will be added to the queue
void RecurseArray(int n, int* rowArray, int startIndex, list<gridNode> checkList, int depth, queue<list<gridNode> >& myQueue, list<gridNode> treeList)
{
    int i;
    int j;
    int iter;

    for (i = startIndex;i<n;i++)
    {
        if (rowArray[i]==0) //if bad, -9, if tree 2, if 0 OKAY
        {
            int rowRecurse[n];
            memcpy(rowRecurse,rowArray,sizeof(rowRecurse)); //make modifications to the copy

            rowRecurse[i] = 1; //the key part of this function is to place lizards in safe spots
  
            //add this (x,y) lizard location to the linked list, and then to the queue
            list<gridNode> newList = checkList;
            gridNode newNode;
            newNode.x = depth;
            newNode.y = i;
            newList.push_back(newNode);
            myQueue.push(newList);

            //set all future row elements before tree to bad so we won't put lizards there
            j = i + 1;
            if (!(j<n))
                return;

            while (j<n)
                {
                    if (rowRecurse[j]==2)
                        break;
                    if (rowRecurse[j]==0)
                    {
                        rowRecurse[j] = -9;
                    }
                    j++;
                }

            if (j==n-1)
                continue;
            RecurseArray(n,rowRecurse,i+1,newList,depth,myQueue,treeList);
        }
    }
}

//If Goal Test fails, the linked list with the current locations of lizards is passed on to ExpandV2
//The next depth is then created and queued in RecurseArray, which is a recurisve function
//Each depth is a row, as opposed to previous implementations (see "Expand(...)"), where each depth was just one lizard
//This increased the nxn dimensions that BFS could compute in under 5 minutes (per HW spec)
void ExpandV2(queue<list<gridNode> >& myQueue, list<gridNode> checkList, int n, int p, list<gridNode> treeList)
{
    //We store the tree list and lizard list separately to reduce the data stored in the queue
    //The quickest way to determine the "safe" and "unsafe" spots are to re-create the nursery
    //Let's set up the nursery matrix
    int  nursery[n][n];
    memset(nursery,0,sizeof(nursery)); //initialize to 0

    int x,y;
    list <gridNode> :: iterator it;
    int it_count = 0;
    //Checklist has the locations of the lizards
    for(it = checkList.begin(); it != checkList.end(); ++it)
    {
        if (it_count > 0)
        {
            gridNode griddy = *it;
            x = griddy.x;
            y = griddy.y;
            nursery[x][y] = 1; //load lizards into nursery
        }
        it_count++;
    }
    //Treelist all the locations of the trees   
    for(it = treeList.begin(); it != treeList.end(); ++it)
    {
        gridNode griddy = *it;
        x = griddy.x;
        y = griddy.y;
        nursery[x][y] = 2; //load lizards into tree
    }
    
    checkList.front().x++;//we will be expanding the NEXT depth, so incrememnt depth in first node
    int depth = checkList.front().x;

    if (depth>=n) //don't want to exceed #rows in matrix
        return; 

    list<gridNode> blankList = checkList; //we want to also make sure we expand the "blank" case, there might be a situation where the answer has NO lizards in earlier rows
                                        //since this is unlikely to happen often, we'll add it to the queue at the end of the expansion
    int singleRow[n]; //this will be the row at our depth we will expand
    memcpy(singleRow,nursery[depth],sizeof(singleRow));


    int j; //this will be the column we're on
    int i ; //this is the row we are at
    int badFlag; //Inirially set to 1, or false
    int negSlope; 
    int posSlope;

    //this loop will be to pre-determine where lizards are able to be safely placed
    //the safe spots will be marked and passed to a recurisve function, which will then create
    //all the safe combinations possible and add these possibilities to the linked list which will then be queued
    if (depth>0) 
    {
    for (j=0;j<n;j++)
    {
        //check above columns for lizards!
        badFlag = 1;
        i = depth-1;
        while (i>=0)
        {
            if (nursery[i][j]==1)
            {
                badFlag = 0; //a lizard is above, unsafe
                break;
            }
            if (nursery[i][j]==2) //there's a tree in the way, nothing above this needs to be considered
            {
                break;
            }
            i--;
        }

        if (badFlag==0)
        {
            singleRow[j] = -9; //this is a "bad cell" b/c of "bad col" marker
            continue;
        }

        //similarly, check positive diagonals
        i = depth-1;
        posSlope= j+1;
     
        while ((i>=0)&&(posSlope<n))
        {    
            if (nursery[i][posSlope]==1)
            {
                badFlag = 0;
                break;
            }

            if (nursery[i][posSlope]==2)
            {
                break;
            }

            i--;
            posSlope++;
        }

        if (badFlag==0)
        {
            singleRow[j] = -9;
            continue;
        }

        //similarly, check negative diagonlas
        i = depth-1;
        negSlope= j-1;
     
        while ((i>=0)&&(negSlope>=0))
        {    
            if (nursery[i][negSlope]==1)
            {
                badFlag = 0;
                break;
            }

            if (nursery[i][negSlope]==2)
            {
                break;
            }

            i--;
            negSlope--;
        }

        if (badFlag==0)
        {
            singleRow[j] = -9;
            continue;
        }
    }
    }
    //at this point, you should have your row marked up properly, and can pass it along
    RecurseArray(n,singleRow,0,checkList,depth,myQueue,treeList);
    if (treeList.size()!= 0)
    {
    	myQueue.push(blankList);//remember the blank row?  we'll queue it up now.
    } 
    return ;
}

void RecurseArrayDFS(int n, int* rowArray, int startIndex, list<gridNode> checkList, int depth, deque<list<gridNode> >& myQueue, list<gridNode> treeList)
{
    int i;
    int j;
    int iter;

    for (i = startIndex;i<n;i++)
    {
        if (rowArray[i]==0) //if bad, -9, if tree 2, if 0 OKAY
        {
            int rowRecurse[n];
            memcpy(rowRecurse,rowArray,sizeof(rowRecurse)); //make modifications to the copy

            rowRecurse[i] = 1; //the key part of this function is to place lizards in safe spots
  
            //add this (x,y) lizard location to the linked list, and then to the queue
            list<gridNode> newList = checkList;
            gridNode newNode;
            newNode.x = depth;
            newNode.y = i;
            newList.push_back(newNode);
            myQueue.push_front(newList);

            //set all future row elements before tree to bad so we won't put lizards there
            j = i + 1;
            if (!(j<n))
                return;

            while (j<n)
                {
                    if (rowRecurse[j]==2)
                        break;
                    if (rowRecurse[j]==0)
                    {
                        rowRecurse[j] = -9;
                    }
                    j++;
                }

            if (j==n-1)
                continue;
            RecurseArrayDFS(n,rowRecurse,i+1,newList,depth,myQueue,treeList);
        }
    }
}

void ExpandV2DFS(deque<list<gridNode> >& myQueue, list<gridNode> checkList, int n, int p, list<gridNode> treeList)
{
    //We store the tree list and lizard list separately to reduce the data stored in the queue
    //The quickest way to determine the "safe" and "unsafe" spots are to re-create the nursery
    //Let's set up the nursery matrix
    int  nursery[n][n];
    memset(nursery,0,sizeof(nursery)); //initialize to 0

    int x,y;
    list <gridNode> :: iterator it;
    int it_count = 0;
    //Checklist has the locations of the lizards
    for(it = checkList.begin(); it != checkList.end(); ++it)
    {
        if (it_count > 0)
        {
            gridNode griddy = *it;
            x = griddy.x;
            y = griddy.y;
            nursery[x][y] = 1; //load lizards into nursery
        }
        it_count++;
    }
    //Treelist all the locations of the trees   
    for(it = treeList.begin(); it != treeList.end(); ++it)
    {
        gridNode griddy = *it;
        x = griddy.x;
        y = griddy.y;
        nursery[x][y] = 2; //load lizards into tree
    }
    
    checkList.front().x++;//we will be expanding the NEXT depth, so incrememnt depth in first node
    int depth = checkList.front().x;

    if (depth>=n) //don't want to exceed #rows in matrix
        return; 

    list<gridNode> blankList = checkList; //we want to also make sure we expand the "blank" case, there might be a situation where the answer has NO lizards in earlier rows
                                        //since this is unlikely to happen often, we'll add it to the queue at the end of the expansion
    int singleRow[n]; //this will be the row at our depth we will expand
    memcpy(singleRow,nursery[depth],sizeof(singleRow));


    int j; //this will be the column we're on
    int i ; //this is the row we are at
    int badFlag; //Inirially set to 1, or false
    int negSlope; 
    int posSlope;

    //this loop will be to pre-determine where lizards are able to be safely placed
    //the safe spots will be marked and passed to a recurisve function, which will then create
    //all the safe combinations possible and add these possibilities to the linked list which will then be queued
    if (depth>0) 
    {
    for (j=0;j<n;j++)
    {
        //check above columns for lizards!
        badFlag = 1;
        i = depth-1;
        while (i>=0)
        {
            if (nursery[i][j]==1)
            {
                badFlag = 0; //a lizard is above, unsafe
                break;
            }
            if (nursery[i][j]==2) //there's a tree in the way, nothing above this needs to be considered
            {
                break;
            }
            i--;
        }

        if (badFlag==0)
        {
            singleRow[j] = -9; //this is a "bad cell" b/c of "bad col" marker
            continue;
        }

        //similarly, check positive diagonals
        i = depth-1;
        posSlope= j+1;
     
        while ((i>=0)&&(posSlope<n))
        {    
            if (nursery[i][posSlope]==1)
            {
                badFlag = 0;
                break;
            }

            if (nursery[i][posSlope]==2)
            {
                break;
            }

            i--;
            posSlope++;
        }

        if (badFlag==0)
        {
            singleRow[j] = -9;
            continue;
        }

        //similarly, check negative diagonlas
        i = depth-1;
        negSlope= j-1;
     
        while ((i>=0)&&(negSlope>=0))
        {    
            if (nursery[i][negSlope]==1)
            {
                badFlag = 0;
                break;
            }

            if (nursery[i][negSlope]==2)
            {
                break;
            }

            i--;
            negSlope--;
        }

        if (badFlag==0)
        {
            singleRow[j] = -9;
            continue;
        }
    }
    }
    //at this point, you should have your row marked up properly, and can pass it along
    RecurseArrayDFS(n,singleRow,0,checkList,depth,myQueue,treeList);
    if(treeList.size()!=0) //don't need to do this if no trees
    {
    	myQueue.push_back(blankList); //remember the blank row?  we'll queue it up now.
    }

    return ;
}

//This is a translation for the pseudo-code for DFS that was provided in class
//Very similar to BFS implementation (see those notes) except that we will be completely expanding one state first until we can no longer expand
//As a result, we will use the same C++ STL Queue implementation but will make it LIFO
int DFS (int n, int p, list<gridNode> treeList, time_t now)
{
   // cout << "In DFS " << endl;
    deque<list<gridNode> > myQueue;
    list<gridNode> myList;
    gridNode newNode; 
    //the first node in the linked list will store the depth infomation in the "x" value.  
    //y is negative just so it doesn't get confused as a nursery location
    newNode.x = -1;
    newNode.y = -1;
    myList.push_back(newNode);
    myQueue.push_front(myList); //add empty list (no lizards yet)

    for(;;)
    {
    	time_t inFunc;
        if (myQueue.empty()||(difftime(time(&inFunc),now)>285))
        {
       //     cout << "FAIL" << endl;
         //   cout << difftime(time(&inFunc),now) << endl;
            fileOutputFail();
            return 1;
        }

        list<gridNode> checkList = myQueue.front();
        myQueue.pop_front(); // can delete this list now
      //  printNursery(checkList,treeList,n);
      //  cout << endl;
        if (GoalTest(checkList,p)==0)
        {
        //    cout << "OK" << endl;
          //  printNursery(checkList,treeList,n);
            fileOutputSuccess(checkList,treeList,n);
            return 0;
        }
        ExpandV2DFS(myQueue,checkList,n,p,treeList);
    }
}

//This is a translation for the pseudo-code for BFS that was provided in class
//The ExpandV2 function will continue to create new states at new depths and queue them
//This BFS function will pop the states off the queue and determine if they meet the goal (implicit) or not
//If not, BFS will call expand to further search subsequent depths of that state
//The BFS Queue is FIFO
int BFS(int n, int p, list<gridNode> treeList, time_t now)
{
   // cout << "IN BFS " << endl;
    queue<list<gridNode> > myQueue;
    list<gridNode> myList;
    gridNode newNode; 
    //the first node in the linked list will store the depth infomation in the "x" value.  
    //y is negative just so it doesn't get confused as a nursery location
    newNode.x = -1;
    newNode.y = -1;
    myList.push_back(newNode);
    myQueue.push(myList); //add empty list (no lizards yet)
    int v1 = 0;
    int depth = -1;
 
    for(;;)
    {
    	time_t inFunc;
        if (myQueue.empty() || (difftime(time(&inFunc),now)>285)) //if the  queue is empty, we have explored all states and nothing works
        {
          //  cout << "FAIL" << endl;
        //    cout << difftime(time(&inFunc),now) << endl;
            fileOutputFail();   
            return 1;
        }

        list<gridNode> checkList = myQueue.front(); //pop it off the front
        myQueue.pop(); // can delet this list now (front() only references, does not delete unlike in Java)
        if (GoalTest(checkList,p)==0) //the goal will check how long the linked list is, since we only add lizards onto "safe" spots
        {
           // cout << "OK" << endl;
            fileOutputSuccess(checkList,treeList,n);
           // printNursery(checkList,treeList,n);
            return 0;
        }
     
        ExpandV2(myQueue,checkList,n,p,treeList);
    }
}

int EnergyLevel(int n, int **nursery)
{
    int i;
    int j;
    //check rows 
    int energy = 0;
    int numberRowLizards = 0;
    int numberColLizards = 0;
    int numberDiagNeg = 0;
    int numberDiagPos = 0;
    //check rows & columns
    for (i=0;i<n;i++)
    {
        for (j=0;j<n;j++)
        {
            if (nursery[i][j] == 1)
            {
                numberRowLizards++;
                if (numberRowLizards>1)
                {
                    energy++;
                    //cout << "energy increased here 1" << endl;
                }
            }
            if (nursery[i][j] == 2)
            {
                numberRowLizards = 0;
            }

            if (nursery[j][i] == 1)
            {
                numberColLizards++;
                if (numberColLizards>1)
                {
                    //cout << "energy increased here 2" << endl;
                    energy++;
                }
            }
            if (nursery[j][i] == 2)
            {
                numberColLizards = 0;
            }
        }
        numberColLizards = 0;
        numberRowLizards = 0;
    }

    //check both positive and negative slopes from left-most column
    int checkdiag_i;
    for (i=0;i<n;i++)
    {
        //check positive slope
        checkdiag_i = i;
        j=0;
        while((checkdiag_i>=0)&&(j<n))
        {
            if(nursery[checkdiag_i][j]==1)
            {
                numberDiagPos++;
                if (numberDiagPos>1)
                {
                //  cout << "energy increase here 3" << endl;
                    energy++;
                }
            }

            if (nursery[checkdiag_i][j] == 2)
            {
                numberDiagPos = 0;
            }
            checkdiag_i--;
            j++;
        }
        numberDiagPos = 0;

        checkdiag_i = i;
        j=0;
        while((checkdiag_i<n)&&(j<n))
        {
            if(nursery[checkdiag_i][j]==1)
            {
                numberDiagNeg++;
                if (numberDiagNeg>1)
                {
                //  cout << "energy increased here 4" << endl;
                    energy++;
                }
            }
            if (nursery[checkdiag_i][j] == 2)
            {
                numberDiagNeg = 0;
            }
            checkdiag_i++;
            j++;
        }
        numberDiagNeg = 0;      
    }

    //check both positive and negative slopes from right-most column
    numberDiagNeg = 0;
    numberDiagPos = 0;
    for (i=1;i<n-1;i++)
    {
        //check positive slope
        checkdiag_i = i;
        j=n-1;
        while((checkdiag_i>=0)&&(j>=0))
        {
            if(nursery[checkdiag_i][j]==1)
            {
                numberDiagPos++;
                if (numberDiagPos>1)
                {
                //  cout << "energy increase here 5" << endl;
                    energy++;
                }
            }

            if (nursery[checkdiag_i][j] == 2)
            {
                numberDiagPos = 0;
            }
            checkdiag_i--;
            j--;
        }
        numberDiagPos = 0;

        checkdiag_i = i;
        j=n-1;
        while((checkdiag_i<n)&&(j>=0))
        {
            if(nursery[checkdiag_i][j]==1)
            {
                numberDiagNeg++;
                if (numberDiagNeg>1)
                {
            //      cout << "energy increased here 6" << endl;
                    energy++;
                }
            }
            if (nursery[checkdiag_i][j] == 2)
            {
                numberDiagNeg = 0;
            }
            checkdiag_i++;
            j--;
        }
        numberDiagNeg = 0;      
    }
    return energy;
}

bool BoltzmanDecides(double T, double deltaE)
{
    deltaE = -1.0*deltaE;
    double P = exp(deltaE/T);
    //if T is very large, -dE/T will be very small, and e^(-dE/T) will be ~1
    //accept this bad move with high probability

    //if T is very small, -dE/T will be very large, and e^(-dE/T) will be ~0
    //accept this bad move with low probability 

    double r = ((double) rand())/RAND_MAX;
    if (r<P)
    {
    //  cout << "Boltzman accepted " <<endl;
    //  cout << "P was " << P << " and r was " << r << endl;
        return true;
    }
    else
    {
    //  cout << "Boltzman DENIED " <<endl;
    //  cout << "P was " << P << " and r was " << r << endl;
        return false;
    }
}

double ScheduleT(double T, double depth)
{
    T = T/(log(depth)/log(10)); //this works for 10 yes
    //T = T/log(depth);
    //T = T  - 0.05;
    return T;
}

int SimulatedAnnealing(int n, int p, int** nursery, double T, time_t now)
{
    //Step 1.  Add random Lizards! 
    int new_row, new_col;
    int row, col;
    int cur_energy = 0;
    int new_energy;
    int next_energy = 0;
    int lizards = 0;
    int i,j,k;

    while (lizards < p)
    {
        row = rand() % n; //in range of 0 to n-1
        col = rand() % n;
        if ((nursery[row][col]!=1)&&(nursery[row][col]!=2))
        {
            nursery[row][col] = 1; // place lizard
            lizards++;
        }
    }
  //  printGrid(n,nursery);

    //Step 2. Count the initial number of conflicts!
    cur_energy = EnergyLevel(n,nursery);

    if (cur_energy == 0)
    {
      //  cout << "OK" << endl;
     //   printGrid(n, nursery);
        fileOutputSuccess_SA(nursery,n);
        return 0;
    }

   //Step 3. Schedule T!
   int depth = 2;
    int q = 2;
    int count = 0;

   //Create a copy of the nursery you can update later
   int **new_nursery = new int*[n];
    for (k=0;k<n;k++)
    {
        new_nursery[k] = new int[n];
    }
    for (i=0;i<n;i++)
    {
        for (j=0;j<n;j++)
        {
            new_nursery[i][j] = nursery[i][j];
        }
    }

   for (;;)
    {
    	time_t inFunc;
    	if (difftime(time(&inFunc),now)>285)
    	{
    		fileOutputFail();
    		return 1;
    		//memory management
    	}
        T = ScheduleT(T,depth);
        //Step 4. select a random successor of current board 
        //let up be 1, right be 2, down be 3, left be 4
        bool moved = false;
        int moveDirection;
        int moveAmount;
        int moveAmountDiag;
        int busy_iter = 0;

        while (moved == false)
        {
        	busy_iter++;
            moveDirection = rand() % 8 + 1;
            row = rand() % n;
            col = rand() % n;
            new_row = row;
            new_col = col;
            if (count < (n-3))
            {
                if (depth % 100 == 0){
                    q++;
                    count++;
                }
                if (busy_iter > (3*n))
                {
                	q++;
                	count++;
                }
            }

            moveAmount = rand() % q + 1;
            moveAmountDiag = rand() % q + 1;
          //  cout << "your random move amount is " << moveAmount << endl;
           // cout << "q is " << q << endl;
           // cout << "the mod is " << depth % 100 << endl;
           // cout <<" move direction: " << moveDirection << " row: " << row << " col: " << col << endl;
            int x;
           // cin >> x;
            if (nursery[row][col]!=1)
            {
                continue;
            }

            if (moveDirection == 1)
            {
                new_row = row + moveAmount;
            }

            if (moveDirection == 2)
            {
                new_col = col + moveAmount;
            }

            if (moveDirection == 3)
            {
                new_row = row - moveAmount;
            }

            if (moveDirection == 4)
            {
                new_col = col - moveAmount;
            }

            if (moveDirection == 5)
            {
                new_row = row  + moveAmount;
                new_col = col + moveAmountDiag;
            }
            if (moveDirection == 6)
            {
                new_row = row  + moveAmount;
                new_col = col - moveAmountDiag;
            }
            if (moveDirection == 7)
            {
                new_row = row  - moveAmount;
                new_col = col + moveAmountDiag;
            }
            if (moveDirection == 8)
            {
                new_row = row  - moveAmount;
                new_col = col - moveAmountDiag;
            }

            if ((0<=new_col)&&(new_col<n)&&(0<=new_row)&&(new_row<n)&&(nursery[new_row][new_col]!=1)&&(nursery[new_row][new_col]!=2))
            {
                //update new nursery
                moved = true;
                busy_iter = 0;
           //   cout << "\nHere's your current nursery: " << endl;
            //  printGrid(n,nursery);
                
                new_nursery[row][col] = 0;
                new_nursery[new_row][new_col] = 1;
            //  cout << "\nNew nursery up for testing: " << endl;
                
            //  cout <<"Old row: " << row << " Old col: " << col << endl;
            //  cout <<"New row: " << new_row << " New col: " << new_col << endl;
                //printGrid(n,new_nursery);

                //check energy
                new_energy = EnergyLevel(n,new_nursery);
              //  cout << "\nCurrent energy level: " << cur_energy << endl;
              //  cout << "New energy level: " << new_energy << endl;
                if (new_energy<=cur_energy)
                {

                //  cout << "\nYou have accepted the change: " << endl;
                    nursery[row][col] = 0;
                    nursery[new_row][new_col] = 1;
                    //moved = true;
                    depth++;
                    cur_energy = new_energy;
                    if (cur_energy == 0)
                    {
                  //      cout << "OK depth is " << depth << endl;
                  //      printGrid(n, nursery);
                        fileOutputSuccess_SA(nursery,n);
                        //memory management
                        return 0;
                    }
                }

                else 
                {
                    if(BoltzmanDecides(T,new_energy - cur_energy)==true)
                    {
                   //     cout << "You have accepted the change based on boltzman: " << endl;
                        nursery[row][col] = 0;
                        nursery[new_row][new_col] = 1;
                        cur_energy = new_energy;
                    //  moved = true;
                        depth++;
                    //  printGrid(n,nursery);
                    } 
                    else
                    {
                    //  cout << "You have NOT accepted the change based on boltzman: " << endl;
                        new_nursery[new_row][new_col] = 0;
                        new_nursery[row][col] = 1;
                        depth++;
                    //  printGrid(n,nursery);
                    }                   
                }
            }


        }
    }
   // cout << " depth is " << depth << endl;
    return 1;
}

//Get grid size, number of lizards, algorithm type
//Initiate BFS, DFS, or SA
void fileInput(time_t now) 
{
    ifstream in_stream;
    in_stream.open("input.txt");
    int n, p;
    string algoType;
    in_stream >> algoType;
    in_stream >> n;
    in_stream >> p;
    //cout << "The algorithm you will use is " << algoType << endl;
    //cout << "You entered " << p << " lizards for a  " << n << "X" << n << " grid." << endl;

    //create a nursery matrix
    int **nursery = new int*[n];
    int k;
    for (k=0;k<n;k++)
    {
        nursery[k] = new int[n];
    }

    //populate the nursery matrix
    //only BFS and DFS need tree linked lists, SA doesn't, but we will use for an SA edge case (not enough empty spots)
    int i, j;
    string inputRow;

    //we don't need to update nursery, just save lizard locations
    
    list<gridNode> treeList;
    for (i=0;i<n;i++)
    {
        in_stream >> inputRow;
        for (j=0;j<n;j++)
        {
        	nursery[i][j] = (inputRow.at(j) - '0');
            if (inputRow.at(j)=='2')
            {
                gridNode newNode;
                newNode.x = i;
                newNode.y = j;
                treeList.push_back(newNode);
            }               
        }
    }
    
   // cout << "tree list size is " << treeList.size() << endl;
      

    //first test edge cases
    //if the number of spaces for lizards isn't enough, fail and quit
    if (((n*n)-treeList.size())<p)
    {
  //  	cout <<" FAIL tree " <<endl;
    	fileOutputFail();
    	return;
    }

    //if the number of lizards is less the the grid dimension
    if ((n*n)<p)
    {
    //	cout <<" FAIL" <<endl;
    	fileOutputFail();
    	return;
    }

    //if no trees and p is greater than n, fails
    if ((treeList.size()==0)&&(n>p))
    {
        fileOutputFail();
        return;
    }
    
    if (algoType == "SA")
    {
      //  cout << "You are entering this grid: " << endl;
       // printGrid(n,nursery);
       // cout << endl;
       // cout << "starting SA ..." << endl;
        SimulatedAnnealing(n,p,nursery,1000,now);
    }

    if (algoType == "BFS")
    {
     //   cout << "starting BFS ..." << endl;
     	BFS(n, p, treeList,now);
    }
    if (algoType == "DFS")
    {
    //    cout << "starting DFS ..." << endl;
     	DFS(n, p, treeList,now);
    }

    //memory management
    for (i=0;i<n;i++)
    {
        delete[] nursery[i];
    }
    delete[] nursery;
    in_stream.close();
}

//In order to not have to physically write input.txt file each time for testing
void createFile()
{
    ofstream myfile;
    myfile.open("input.txt");
    string algo;
    int nn;
    int pp;
    int u,z;
    cout << "Enter algo type" << endl;
    cin >> algo;
    myfile << algo << endl;

    cout << "Enter grid size (nxn)" << endl;
    cin >> nn;
    cout << "Enter lizards (p)" << endl;
    cin >> pp;
    myfile << nn <<endl;
    myfile << pp << endl;
    for (u=0;u<nn;u++)
    {
        for(z=0;z<nn;z++)
        {
            myfile<<"0";
        }
        myfile << endl;
    }
    myfile.close();
}

//Run BFS, DFS, or SA using fileInput()
int main()
{
	//createFile();
	time_t now;
//	time_t later;
	time(&now); //store current time
//	cout << "time is " << now << endl;	
    fileInput(now);
  //  time(&later);
   // cout << "time is " << difftime(later,now) << endl;
    return 0;
}


