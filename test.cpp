#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <algorithm>
#include <vector>
#include <mutex>
#include <time.h>
#include <queue>
using namespace std;
mutex p_mutex;



struct process{
    private:
        int Id;//Id del proceso
        int Priority;//Prioridad del proceso
        int TurnAroundTime;//Tiempo que tarda el proceso desde que llega hasta que se termina de ejecutar
        int ResponseTime;//Tiempo que tarda el proceso desde que llega hasta que se ejecuta por primera vez
        
        int ArriveTime;//Tiempo en el que llego proceso
        int FinishTime;//Tiempo en que el proceso termino de ejecutarse
    public:
        int ExecutionTime;//Tiempo de ejecucion que necesita el proceso para ejecutarse
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
            this->ArriveTime=rand()%100;
            this->ExecutionTime=rand()%100;
        }
        
        //~process();
};

struct ShortestJob{
    bool operator()(process const &p, process const &p1){
        return p.ExecutionTime > p1.ExecutionTime;
    }
};
/*Clase que representa una RunQueue*/
class RunQueue{
    private:
        vector<priority_queue<process, vector<process>, ShortestJob>> queue;//Vector de buckets de procesos
    public:
    /*Constructor que recibe el algoritmo de planificacion que se empleara*/
        RunQueue(int n){
            for(int i = 0; i < n; ++i){
                priority_queue<process, vector<process>, ShortestJob> v1;
                this->queue.push_back(v1);
            }
        }
        /*Metodos insert y delete*/
        void insert(process p){
            this->queue[p.getPriority()].push(p);
        }
        void delete_process(process p){
            this->queue[p.getPriority()].pop();
        }
        /*Debug*/
        int getSize(){
            return this->queue.size();
        }
        void getElems(int pos){
            for(int i = 0; i < this->queue[pos].size(); ++i){
                while(!queue[pos].empty()){
                    cout<<queue[pos].top().ExecutionTime<<'\n';
                    queue[pos].pop();
                }
            }
        }
        /*********/
        ~RunQueue(){
            this->queue.clear();
        }
};

void tomarProceso(){
    p_mutex.lock();
    p_mutex.unlock();
}

void crearProcesos(int n, RunQueue &q){
    for(int i = 0; i < n; ++i){
        process a(i);
        q.insert(a);
    }
}

void leerInfoProceso(){
    p_mutex.lock();
    p_mutex.unlock();

}
/*
    Argumentos: 
    N = Numero de procesos
    M = Cantidad de hebras especializadas
    Q = Quantum
    MaxT= Tiempo de ejecucion maxima(?)
*/
int main(int argc, char *argcv[]){
    
    if(argc != 5){
        cout<<"Faltan argumentos we.\n";
        return 0;
    }

    srand(time(NULL));
    RunQueue q(10);
    thread gestion(crearProcesos,argcv[1],ref(q));
    gestion.join();
   // cout<<q.getSize()<<'\n';
   //cout<<a.getArriveTime()<<endl;
    q.getElems(0);
    return 0;
}


