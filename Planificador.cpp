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

int test=0;
int id =0;
int N =0;
int M=0;
int Q=0;
int MaxT=0;
int contT = 0;
int flag=0;
int flag2=0;
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
        int getTurnAroundTime(){
            return this->TurnAroundTime = this->FinishTime - this->ArriveTime;
        }
        int getResponseTime(){//Le da el valor a ResponseTime, en funcion al tiempo en que llego a la queue y cuando se ejecuta el proceso
            return this->ResponseTime;
        }
        void setResponseTime(int n){
            this->ResponseTime =  n-this->ArriveTime;
        }
        void updateFinishTime(){
            this->FinishTime=contT;
        }
        void updatePriority(){
            if(this->Priority<9 && this->Priority>=0){
                this->Priority++;
            }
        }

        void setFinishTime(){
            if(this->ExecutionTime=0){//crear cuando tengamos el Quantum definido
            }
        }

        
        process(int i, int time){
            this->Id=i;
            this->Priority=0;
            this->FinishTime=0;
            this->ArriveTime=time;
            this->ResponseTime=0;
            this->TurnAroundTime=0;
            this->ExecutionTime=rand()%MaxT+1;
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
        vector<priority_queue<process, vector<process>, ShortestJob>> queue;
        vector<priority_queue<process, vector<process>, ShortestJob>> queue_exp;//Vector de buckets de procesos
    public:
    /*Constructor que recibe el algoritmo de planificacion que se empleara*/
        RunQueue(int n){
            for(int i = 0; i < n; ++i){
                priority_queue<process, vector<process>, ShortestJob> v1;
                this->queue.push_back(v1);
                this->queue_exp.push_back(v1);
            }
        }
        /*Metodos insert y delete*/
        void insert(process p){
            this->queue[p.getPriority()].push(p);
        }
        void insertexp(process p){
            this->queue_exp[p.getPriority()].push(p);
        }
        void delete_process(process p){
            this->queue[p.getPriority()].pop();
        }
        /*Debug*/
        void getElems(int pos){
            for(int i = 0; i < this->queue[pos].size(); ++i){
                while(!queue[pos].empty()){
                    cout<<queue[pos].top().ExecutionTime<<'\n';
                    queue[pos].pop();
                }
            }
        }
        int getPos(){
            for(int i = 0; i < 10; ++i){
                if(this->queue[i].size()){
                    return i;
                }
            }
            return -1;
        }
        int getSize(){
            int size = 0;
            for(auto i : this->queue){
                size += i.size();
            }
            return size;
        }
        int getSizeExp(){
            int size = 0;
            for(auto i : this->queue_exp){
                size += i.size();
            }
            return size;
        }
        
        int getPosExp(){
            for(int i = 0; i < 10; ++i){
                if(this->queue_exp[i].size()){
                    return i;
                }
            }
            return -1;
        }
        process getTop(int pos){
            return this->queue[pos].top();
        }
        void swap_queue(){
            this->queue=this->queue_exp;
            this->queue_exp.clear();
            for(int i = 0; i < 10; ++i){
                priority_queue<process, vector<process>, ShortestJob> v1;
                this->queue_exp.push_back(v1);
            }
        }
        /*********/
        ~RunQueue(){
            this->queue.clear();
            this->queue_exp.clear();
        }
};
RunQueue q(10);
void crearProcesos(){
    for(int i = 0; i < N; ++i){
        process a(i, contT);
        q.insert(a);
    }
}
void tomarProceso(){
    process p(id++,contT);
    while(1){
        p_mutex.lock();
        int Quantum = Q;
        /*Busca la primera cola con prioridad i = 0 hasta i = 9 no vacia*/
        int pos = q.getPos();
        int pos_exp = q.getPosExp();
        //cout<<"posicion: "<<pos<<endl;
        if(pos >= 0){
            p = q.getTop(pos);
            q.delete_process(p);
            
        }else if(q.getSizeExp()){
    
            q.swap_queue();
            pos = q.getPos();
            p = q.getTop(pos);
            q.delete_process(p);
        }else{
            //cout<<"-------------------------------------entra al return----------------------------------------------------------"<<endl<<endl<<endl<<endl<<endl<<endl<<endl;

            
            p_mutex.unlock();
            
            return;
        }
        cout<<"Ejecutando pid: "<<p.getId()<<endl;
        if(!p.getResponseTime()){
            p.setResponseTime(contT);    
        }
        p_mutex.unlock();
        while(Quantum!=0){
            Quantum--;
            if(p.ExecutionTime > 0) p.ExecutionTime -= 1;
            p_mutex.lock();
            contT++;
            p_mutex.unlock();
        };
        p_mutex.lock();
        cout<<"Termino el quantum para el proceso: "<< p.getId()<<" con tiempo restante: "<<p.ExecutionTime<<" y prioridad: "<<p.getPriority()<<endl<<endl;
        if(!p.getExecutionTime()){
            p.updateFinishTime();
            cout<<"---------------Se termino de ejecutar el proceso: "<<p.getId()<<"---------------"<<endl;
            cout<<"Tiempo de llegada: "<<p.getArriveTime()<<endl;
            cout<<"Tiempo de respuesta: "<<p.getResponseTime()<<endl;
            cout<<"TurnAround time: "<<p.getTurnAroundTime()<<endl;
            cout<<"Prioridad Final: "<<p.getPriority()<<endl;
            cout<<"------------------------------------------------------------------"<<endl<<endl;

        }
        if(p.ExecutionTime > 0){
            p.updatePriority();
            q.insertexp(p);
        }
        p_mutex.unlock();
    }
}



void leerInfoProceso(){
    p_mutex.lock();
    p_mutex.unlock();

}

void ejecutar_simulacion(){
    vector<thread> vm;
    for(int i = 0; i < M; ++i){
        thread t1(tomarProceso);
        vm.push_back(std::move(t1));
    }
    for(int i = 0; i < M; ++i){
        vm.at(i).join();
    }
}
/*
    Argumentos: 
    N = Numero de procesos
    M = Cantidad de hebras especializadas
    Q = Quantum
    MaxT= Tiempo de ejecucion maxima
*/  
int nuevasHebras(){
char c;
cout<<"Quieres seguir ejecutando procesos? Y/N"<<endl;
cin>>c;
while(c!='n'&&c!='N'&&c!='y'&&c!='Y'){
    cout<<"Entrada incorrecta"<<endl;
    cin>>c;
    }
    if(c=='y'||c=='Y'){
        thread hebraG(crearProcesos);
        hebraG.join();
        test=1;
        return 0;
    }
    test=0;
    return  0;
}
int main(int argc, char *argcv[]){
    srand(time(NULL));
    if(argc != 5){
        cout<<"Faltan argumentos we.\n";
        return 0;
    }
    N = stoi(argcv[1]);
    M = stoi(argcv[2]);
    Q = stoi(argcv[3]);
    MaxT = stoi(argcv[4]);
    thread gestion(crearProcesos);
    gestion.join();
    ejecutar_simulacion();
    while(test==1||flag2==0){
        flag2=1;
        thread Gaux(nuevasHebras);
        Gaux.join();
        ejecutar_simulacion();
    }
    return 0;
}


