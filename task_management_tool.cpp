/*
 @Author
 Sudent Name: Selin Eylul Bilen
 Date: 15.11.2019
 */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot =new Task;
		Task *compeer =new Task;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				//if(compeer->next){cout << "t\n";			cout << compeer->next->day<<"\n";cout <<compeer->next->name<<"\n";}

				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL ? compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{	
	return usable_day;
}

int WorkPlan::getUsableTime()
{	

	return usable_time;
}


void WorkPlan::create()
{		
	//THIS FUNCTION WILL BE CODED BY YOU
	head=0;
	usable_day=0;
	usable_time=0;
	//DONE
}

void WorkPlan::close()
{
	//THIS FUNCTION WILL BE CODED BY YOU
	//cout << "y\n";
	while(head){
		remove(head->next);
	}

}

void WorkPlan::add(Task *task)
{
	//THIS FUNCTION WILL BE CODED BY YOU
	//todo cakismalar;
	//cout << "add"<<task->day<<task->time<<" "<<task->next<<" "<<task->previous<<" "<<task->counterpart<<" "<<"\n";

	bool nodayforit=true;
	
	if(!head){								//bombos
		//cout << "ilk\n";
		head = new Task;			
		head->name = new char[strlen(task->name)];
		strcpy(head->name,task->name);
		head->day = task->day;         		//day
		head->time = task->time;			//time
		head->priority = task->priority;	//priority
		head->next = head;					//next
		head->previous = head;				//previous
		head->counterpart=NULL;
	}
	else{  // there is already a head
		Task * temp = head;
		do{
			if(temp -> day == task -> day){
				nodayforit = false;
				break;
			}
			temp=temp->next;
		}
		while(temp != head);
		temp=head;
		if(!nodayforit){ //day var yani yeni bir day'e gerek yok
			while(temp->day != task->day)
				temp=temp->next;    	//tempin day'i gerekli day'e esit

			if(temp->time > task->time){			//TODONE - TASK MUST BE TAKEN TO FRONT
				Task * tmp = new Task;
				tmp->name = new char[strlen(task->name)];
				strcpy(tmp->name,task->name);
				tmp->day = task->day;
				tmp->time = task->time;			//time
				tmp->priority = task->priority;

				tmp->next 	= 	temp->next;
				tmp->previous	=	temp->previous;
				temp->previous->next	=	tmp;
				temp->next->previous 	=	tmp;
				
				tmp->counterpart=temp;
				temp->next=NULL;
				temp->previous=NULL; //important parts, DON'T FORGET
				
				if(head == temp){
					head = tmp;
				}

			}
			else if(temp->time == task->time){ 	// TODONE - CAKISIYOR PRIORITYLERE GORE BIR YERE EKLEMELISIN
				//cout << "CAKISMAA"<<task->day<< " "<<task->time <<"\n";
				//TODONE
				int tspriority=task->priority;
				int tmpriority=temp->priority;
				if(tspriority <= tmpriority){
					checkAvailableNextTimesFor(temp);
					//cout <<"next vailable time="<< usable_day << usable_time<<"\n";
					Task * tmp = new Task;
					
					tmp->name = new char[strlen(task->name)];
					strcpy(tmp->name,task->name);
					
					tmp->priority = task->priority;
					tmp->day = usable_day;
					tmp->time = usable_time;
					
					tmp->next=NULL;
					tmp->previous=NULL;
					tmp->counterpart=NULL;
					add(tmp);
					
					return;
				}
				else{ //you must ertele the older one
					// temp = task'le ayni zamandaki eleman ama tablodaki
					checkAvailableNextTimesFor(temp);
					Task * tmp = new Task;
					tmp->name = new char[strlen(temp->name)];
					strcpy(tmp->name,temp->name);
					tmp->priority=temp->priority;
					tmp->day = usable_day;
					tmp->time = usable_time;
					tmp->next=NULL;
					tmp->previous=NULL;
					tmp->counterpart=NULL;
					add(tmp);

					delete temp->name;
					temp->name=NULL;
					temp->name = new char[strlen(task->name)];
					strcpy(temp->name,task->name);
					temp->priority=task->priority;
					temp->day=task->day;
					temp->time=task->time;
					return;
				}
			}
			else { 								// TODO ILK SAATTEN SONRASINA EKLE
				while(temp->counterpart && temp->counterpart->time < task->time){
					temp = temp->counterpart;
				} //temp bir oncekini gosteriyor veya en son elemani

				if(temp->counterpart != NULL){
					if(temp->counterpart->time == task->time){ //TODONE
						//cout << "CAKISMA"<<task->day<< " "<<task->time <<"\n";
						temp = temp->counterpart;
						int tspriority=task->priority;
						int tmpriority=temp->priority;
						if(tspriority <= tmpriority){
							checkAvailableNextTimesFor(temp);
							//cout <<"next vailable time="<< usable_day << usable_time<<"\n";
							Task * tmp = new Task;
							tmp->name = new char[strlen(task->name)];
							strcpy(tmp->name,task->name);
							tmp->priority=task->priority;
							tmp->day = usable_day;
							tmp->time = usable_time;
							tmp->next=NULL;
							tmp->previous=NULL;
							tmp->counterpart=NULL;
							add(tmp);
							return;
						}
						else{ //you must ertele the older one
							// temp = task'le ayni zamandaki eleman ama tablodaki
							checkAvailableNextTimesFor(temp);
							Task * tmp = new Task;
							tmp->name = new char[strlen(temp->name)];
							strcpy(tmp->name,temp->name);
							tmp->priority=temp->priority;
							tmp->day = usable_day;
							tmp->time = usable_time;
							tmp->next=NULL;
							tmp->previous=NULL;
							tmp->counterpart=NULL;
							add(tmp);

							delete temp->name;
							temp->name=NULL;
							temp->name = new char[strlen(task->name)];
							strcpy(temp->name,task->name);
							temp->priority=task->priority;
							temp->day=task->day;
							temp->time=task->time;
							return;
						}

					}
					else{
						//
						

						Task * tmp = new Task;
						tmp->name = new char[strlen(task->name)];
						strcpy(tmp->name,task->name);
						tmp->day = task->day;
						tmp->time = task->time;			//time
						tmp->priority = task->priority;
						tmp->next=NULL;
						tmp->previous=NULL;
						tmp->counterpart=temp->counterpart;
						temp->counterpart = tmp;
						return;
					}
				}
				else{ //temp son eleman
					if(temp->time==task->time){//ertele //TODONE
						//cout << "CAKISMA"<<task->day<< " "<<task->time <<"\n";
						int tspriority=task->priority;
						int tmpriority=temp->priority;
						if(tspriority <= tmpriority){
							checkAvailableNextTimesFor(temp);
							//cout <<"next vailable time="<< usable_day << usable_time<<"\n";
							Task * tmp = new Task;
							tmp->name = new char[strlen(task->name)];
							strcpy(tmp->name,task->name);
							tmp->priority=task->priority;
							tmp->day = usable_day;
							tmp->time = usable_time;
							tmp->next=NULL;
							tmp->previous=NULL;
							tmp->counterpart=NULL;
							add(tmp);
							return;
						}
						else{ //you must ertele the older one
							// temp = task'le ayni zamandaki eleman ama tablodaki
							checkAvailableNextTimesFor(temp);
							Task * tmp = new Task;
							tmp->name = new char[strlen(temp->name)];
							strcpy(tmp->name,temp->name);
							tmp->priority=temp->priority;
							tmp->day = usable_day;
							tmp->time = usable_time;
							tmp->previous=NULL;
							tmp->next=NULL;
							tmp->counterpart=NULL;
							add(tmp);

							delete temp->name;
							temp->name=NULL;
							temp->name = new char[strlen(task->name)];
							strcpy(temp->name,task->name);
							temp->priority=task->priority;
							temp->day=task->day;
							temp->time=task->time;
							return;
						}
					}
					else{ //bundan sonrasina ekle
						Task * tmp = new Task;
						tmp->name = new char[strlen(task->name)];
						strcpy(tmp->name,task->name);

						tmp->day = task->day;
						tmp->time = task->time;			//time
						tmp->priority = task->priority;
						tmp->next=NULL;
						tmp->previous=NULL;
						tmp->counterpart=NULL;
						temp->counterpart=tmp;
					}
				}
			}
			//TODO
		}
		else{ 	//no day for the appointment, you need to make a new day
			//in order to do that first create a new task, and then find the day before what we must put in the plan

			//finding the day
			//cout << "b\n";
			temp = head;
			if(temp->next == head){ // there is only head TODONE
				Task * tmp = new Task;
				head->next = tmp;
				tmp->next=head;
				head->previous=tmp;
				tmp->previous=head;
				tmp->name = new char[strlen(task->name)];
				strcpy(tmp->name,task->name);

				tmp->day = task->day;
				tmp->time = task->time;			//time
				tmp->priority = task->priority;
				
				if(head->day > tmp->day){
					head = tmp;
				}

			}
			else if(task->day < temp->day ){ // put it before the first appointment TODONE smaller than all of them
				Task *tmp = new Task;
				Task *onceki = head->previous;
				
				tmp->name = new char[strlen(task->name)];
				strcpy(tmp->name,task->name);

				tmp->day = task->day;
				tmp->time = task->time;			//time
				tmp->priority = task->priority;
				
				tmp->next=head;
				onceki->next=tmp;

				tmp->previous=onceki;
				tmp->next->previous = tmp;

				head=tmp;
			}
			else if(head->previous->day < task->day){	//hepsinden buyuk TODONE
				Task *tmp = new Task;
				tmp->name = new char[strlen(task->name)];
				strcpy(tmp->name,task->name);
				tmp->day = task->day;
				tmp->time = task->time;			//time
				tmp->priority = task->priority;

				tmp->next=head;
				head->previous->next=tmp;

				tmp->previous=head->previous;
				tmp->next->previous=tmp;


			}
			else { //TODO
				Task * oncekiday=head; // bi onceki gunu buluyorum bu günden sonraya bizimkini eklicem DONE
				while( oncekiday->next->day <= task->day){
					oncekiday=oncekiday->next;
				}
				Task *tmp = new Task;
				tmp->name = new char[strlen(task->name)];
				strcpy(tmp->name,task->name);
				tmp->day = task->day;
				tmp->time = task->time;			//time
				tmp->priority = task->priority;
				
				tmp->next=oncekiday->next;
				oncekiday->next=tmp;
				
				tmp->previous=oncekiday;
				tmp->next->previous=tmp;

			}

		}
		//TODO
	}
}


Task * WorkPlan::getTask(int day, int time)
{
	//THIS FUNCTION WILL BE CODED BY YOU
	//DONE it maybe doesn't exist done all of them
	if(!head) return NULL;
	Task * a = head;
	while(a->day != day){
		a=a->next;
		if(a==head)
			return NULL;
	}
	while(a->time!=time){
		a=a->counterpart;
		if(!a)
			return NULL;
	}

	return a;
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{	
	//THIS FUNCTION WILL BE CODED BY YOU
	//DONE
	//cout << "check\n";
	Task * temp = head;
	bool noschedule = true;
	Task * tmp = delayed;
	if(!temp){ 
		usable_day=1;
		usable_time=8;
		return; //there is no head
	}
	if(!delayed) return; 
	//there is no task
	
	while(temp->day != delayed->day){
		temp=temp->next;
		//cout << "m\n";
		if(temp==head){ //there is no day for the delayed task
			cout << "THERE IS NO DAY FOR THE DELAYED TASK\n" ;
			return;		//todo - not necessary but check for the next available time even though there is no task in the schedule
		}
	}	//temp aynı güne geldi
	
	while(true){
		if(tmp->counterpart){ //baska elemanlardan once - tamamdir
			if(tmp->counterpart->time - tmp->time == 1){//hemen sonra bir task daha var, erteleyemem-tamamdir
				tmp=tmp->counterpart;
				continue;
			}
			else{ //sonrasinda task yok - tamamdir
				usable_day = tmp->day;
				usable_time = tmp->time+1;
				noschedule = false; //there is schedule
				break;
			}
		}
		else{ //en son eleman
			if(temp->next == head){
				// daha fazla task kalmadi noschedule, ilk yere gidip koyucaz
				break;
			}
			else{
				temp=temp->next;
				if(temp->time > 8){//ilk saat bos yani oraya ekleyebiliriz
					usable_time=8;
					usable_day=temp->day;
					noschedule=false;
					break;
				}
				// else - next day's 8 oc'lock is full;
				tmp=temp;
			}
		}
	}
	if(noschedule){ //there is no available time so the first free hour will be chosen
		temp = head;
		//cout << "b\n";
		while(temp->day != delayed->day){
			temp=temp->next;
			if(temp==head){ //there is no day for the delayed task
				return;		//todo - not necessary but check for the next available time even though there is no task in the schedule
			}
		}	//temp aynı güne geldi
	

		tmp = delayed;
		while(true){
			if(tmp->counterpart){ //tmp counterpart var
				if(tmp->counterpart->time - tmp->time == 1){ //1 saat sonrasi da dolu
					tmp=tmp->counterpart;
				}
				else{ //arada bir saatlik bosluk var
					usable_day=tmp->day;
					usable_time=tmp->time+1;
					break;
				}
			}
			else{ //tmp counterpart yok
				if(tmp->time==16){ //son saat, yeni gune bakman lazim
					if(getTask(temp->day+1,8)!=NULL){
						temp=temp->next;
						tmp=temp;
					}
					else{
						usable_day=temp->day+1;
						usable_time=8;
						break;
					}
				}
				else{
					usable_time=tmp->time+1;
					usable_day=tmp->day;
					break;
				}

			}
		}
	}

}

void WorkPlan::delayAllTasksOfDay(int thatday)
{
	//THIS FUNCTION WILL BE CODED BY YOU
	//TODOTODO WHAT IF THE ONLY DAY IS DELAYED
	// OTHER THAN THAT EVERYTHING IS OKAY
	Task * temp = head;
	if(!head) return; // there is no head;
	while(temp->day!=thatday){
		temp=temp->next;
		if(temp==head) return; // no tasks in that day already
	}
	// temp is equal to the first element of the days in thatday

	Task *p =temp->previous;//bir onceki gun

	while(p->next->day == thatday){
		Task * old = getTask(p->next->day,p->next->time);
		Task * tmp = new Task;
		tmp->name = new char [strlen(old->name)];
		strcpy(tmp->name,old->name);
		checkAvailableNextTimesFor(old);
		tmp->day = usable_day;
		tmp->time = usable_time;
		remove(old);
		add(tmp);
	}

	

}

void WorkPlan::remove(Task *target)
{
	//THIS FUNCTION WILL BE CODED BY YOU
	//DONE
	Task * tmp;
	Task * temp = head;
	if(!target) return; //no target
	if(!head) return; //no head

	if(target==head && !target->counterpart && target->next==target){ //target is the only one
		delete head;
		head=NULL;
		return;
	}
	if(head->next==head){ //there is only one day
		if(head==target){  // there is one day and head is deleted
			head = head->counterpart;
			head->next=head;
			head->previous=head;
			delete target;
			return;
		}
		else{
			while(temp && temp->counterpart!=target){ //tempin counterpart i targeta eşit olana kadar gidecek 
				temp=temp->counterpart;
			}

			//todotodo burada anlamadigim bir sorun var
			if(!temp){
				cout<<"THERE MUST BE AN ERROR NO TARGET IS FOUND\n";
				return;
			}
			tmp=temp->counterpart;
			temp->counterpart=temp->counterpart->counterpart;
			delete tmp;
			return;
		}
	}


	temp=head;
	while(temp->day != target->day){ // targetin dayindeki ilk gun = temp
		temp=temp->next;
		if(temp==head){
			cout << "THERE MUST BE AN ERROR, THE DAY OF THE TARGET DOESN'T EXIST";
			return;
		}
	}

	if(temp==target){ //first time will be removed
		if(temp->counterpart){ //counterpart i var
			temp->previous->next=temp->counterpart;
			temp->next->previous=temp->counterpart;
			temp->counterpart->next=temp->next;
			temp->counterpart->previous=temp->previous;
			delete temp;
		} //counterpart i yok 
		else{
			temp->previous->next=temp->next;
			temp->next->previous=temp->previous;
			delete temp;
		}
		return;
	}
	else{  //not the first time
		bool d = false; //d for deleted
		while(temp && temp->counterpart){
			if(temp->counterpart == target){
				d=true;
				Task * tmp = temp -> counterpart;
				temp->counterpart = temp->counterpart->counterpart;
				delete tmp;
				tmp=0;
				break;
			}
			temp=temp->counterpart;
			//TODO- it maybe doesn't delete anything, you must look at it;
		}
		if(!d){
			cout << "NOTHING IS DELETED, THERE MUST BE ERROR ON TARGET DELETING\n";
		}
	}

	

}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
