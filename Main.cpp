#include <iostream>
#include <thread>
#include <atomic>
#include <algorithm>

using namespace std;

class process{
    private:
        atomic<int> Id;//Id del proceso
        atomic<int> Priority;//Prioridad del proceso
        atomic<int> TurnAroundTime;//Tiempo que tarda el proceso desde que llega hasta que se termina de ejecutar
        atomic<int> ResponseTime;//Tiempo que tarda el proceso desde que llega hasta que se ejecuta por primera vez
        atomic<int> ExecutionTime;//Tiempo de ejecucion que necesita el proceso para ejecutarse
        atomic<int> ArriveTime;//Tiempo en el que llego proceso
        atomic<int> FinishTime;//Tiempo en que el proceso termino de ejecutarse
    public:
        int getId(){//Funcion para obtener el valor de la variable Id
            return this->Id;
        }
        int getPriority(){//Funcion para obtener el valor de la variable Priority
            return this->Priority;
        }
        int getArriveTime(){//Funcion para obtener el valor del tiempo en el que llego el proceso a la cola
            return this->ArriveTime;
        }
        int getExecutionTime(){//Funcion para obtener el valor del tiempo en el que se ejecuto por primera vez el proceso
            return this->ExecutionTime;
        }
        void updatePriority(){
            if(this->Priority<9 && this->Priority>=0){
                this->Priority++;
            }
        }
        void setResponseTime(){//Le da el valor a ResponseTime, en funcion al tiempo en que llego a la queue y cuando se ejecuta el proceso
            this->ResponseTime = this->ArriveTime + this->ExecutionTime;//Revisar
        }
        void setFinishTime(){
            if(this->ExecutionTime=0){//crear cuando tengamos el Quantum definido
            }
        }
        void setTurnAroundTime(){
            this->TurnAroundTime = this->FinishTime - this->ArriveTime;
        }
        
        process(int i){
            this->Id=i;
            this->Priority=0;
            this->FinishTime=0;
            this->ArriveTime=Rand()%100;
            this->ExecutionTime=Rand()%100;
        }
        
        ~process();
}




/*Clase que representa una RunQueue*/
class RunQueue{
    private:
        atomic<vector<vector<process>>> queue;//Vector de buckets de procesos
        string algoritmo; //Indica el algoritmo con el que funciona la queue
        /*Funciones de comparacion para std::sort de las estructuras de proceso*/
        int cmp_fjf(process p, process p1){
            return p.getArriveTime() < p1.getArriveTime();
        }
        int cmp_sjf(process p, process p1){
            return p.getExecutionTime() < p1.getExecutionTime();
        }
        /*Funciones que ordenan los buckets acorde al algoritmo seleccionado*/
        void fjf(){
            for(int i = 0; i < 10; ++i){
                sort(queue[i].begin(),queue[i].end(), this->cmp_fjf());
            }
        }
        void sjf(){
            for(int i = 0; i < 10; ++i){
                sort(queue[i].begin(),queue[i].end(), this->cmp_sjf());
            }
        }
    public:
    /*Constructor que recibe el algoritmo de planificacion que se empleara*/
        RunQueue(string alg){
            this->algoritmo = alg;
            for(int i = 0; i < 10; ++i){
                vector<process> v1;
                this->queue.push_back(v1);
            }
        }
        /*Metodos insert y delete*/
        void insert(process p){
            this->queue[p.getPriority()].push_back(p);
        }
        void delete(process p){
            this->queue[p.getPriority()].erase(queue[p.getPriority()].begin());
        }
        /*Metodo de ordenamiento segun el algoritmo*/
        void sort(){
            if(this->string == SJF){
                this->sjf();
            }else if(this->string == FJF){
                this->fjf();
            }
        }
        ~RunQueue(){
            this->queue.clear();
        }
}


