#include <iostream>
#include "TXLib.h"


class fish {
    public:
    int id = 0;
    int size = 0;
    int x = 0, y = 0;
    int vx = 0;
    int vy = 0;
    int vmax = 0;
    COLORREF color = RGB(255,0,0);
    bool life = true;
    
    // constructor of function C // like init in python
    fish(){
    	
    }  
    fish(COLORREF vx_color,int vx_vmax,int vx_size,int vx_x, int vx_y, int vx_id)
	{
		id = vx_id;
		size=vx_size;
		color=vx_color;
		vmax=vx_vmax;
		x=vx_x;
		y=vx_y;
		vx=rand()%vmax-vmax/2;
		vy=rand()%vmax-vmax/2;
		life=true;
	}
	

	// function describes movement of fish in the next moment, not considering another fishes.
	void interaction(int xmax,int ymax)  
	{
		if(not life)return;
		x+=vx;
		y+=vy;
		if(rand()%25==0)
		{
			vx=(int)((double)vmax*(rand()%100-50)/50)+1;
			vy=(int)((double)vmax*(rand()%100-50)/50)+1;
		}
		// right side 
		if(x+2*size>xmax){
			x=xmax-2*size;
			vx=-vx;
		}
		
		// bottom side
		if(y+2*size>ymax){
			y=ymax-2*size;
			vy=-vy;
		}
		
		// left side
		if(x-2*size<0){  
			x=2*size;
			vx=-vx;	
		}
		
		// top side
		if(y-2*size<0){ 
			y=2*size;
			vy=-vy;
		}
		
	}
	
	// fuction describes interaction of this fish and fish from input params
	void interaction(fish *A) 
	{
		if(not life or not (*A).life)return;
		int Ax=(*A).x;
		int Ay=(*A).y;	
		int Asize=(*A).size;	
		
		 // if summ size of two fishes is less than distance betwen their centers, then ..
		if((x-Ax)*(x-Ax)+(y-Ay)*(y-Ay)<2*(size+Asize)*(size+Asize))
		{
			vx = ((int)(1.0*vmax*(x-Ax)/sqrt(1.0*(x-Ax)*(x-Ax)+(y-Ay)*(y-Ay))));
			vy = ((int)(1.0*vmax*(y-Ay)/sqrt(1.0*(x-Ax)*(x-Ax)+(y-Ay)*(y-Ay))));
			(*A).vx=-vx;
			(*A).vy=-vy;
			x+=2*vx;
			y+=2*vy;
			(*A).x-=2*vx;
			(*A).y-=2*vy;
		}
	}
};

//Predator Fish class
class predatory_fish: public fish 
{	
	public:
	// Predator can folow one of the fish, than it will move not chaotic, but deterministic	
	int focus;
	
	predatory_fish(){}
	predatory_fish(int v_vmax,int v_size,int vx_x, int vx_y, int vx_id)
		{
			
			focus = -1;
			id = vx_id;
			size=v_size;
			color=RGB(255,0,0);
			vmax=v_vmax;
			x=vx_x;
			y=vx_y;
			vx=rand()%vmax+1;
			vy=rand()%vmax+1;
			life=true;
		}
	
		void interaction(fish *A) 
	{
		if(!(*A).life)return;
		
		int Ax=(*A).x;
		int Ay=(*A).y;	
		int Asize=(*A).size;
	  	
	  	//If not Predator more less than Predator and is close to predator, than it will be eaten
	  	if(Asize*1.5<size  and (x-Ax)*(x-Ax)+(y-Ay)*(y-Ay)<2*(size+Asize)*(size+Asize)){
	  		(*A).life=false;
	  		//Focus on fish will be canceled if eaten fish is the fish that had focus on
	  		if(focus ==  (*A).id)focus = -1;
		  }
	  		
	  		
	  	
	  	if(focus == -1){
			//If fish din`t follow anyone and near is little fish than predator starts to follow it
	  		if((*A).life and Asize*1.5<size  and (x-Ax)*(x-Ax)+(y-Ay)*(y-Ay) < 220*220){
	  		
				focus = (*A).id;
				
				//speed vector towards victim
				vx = -((int)(1.0*vmax*(x-Ax)/sqrt(1.0*(x-Ax)*(x-Ax)+(y-Ay)*(y-Ay))));
				vy = -((int)(1.0*vmax*(y-Ay)/sqrt(1.0*(x-Ax)*(x-Ax)+(y-Ay)*(y-Ay))));
				return;
			} 
		}
	  	else{
			    if(focus ==  (*A).id)
		  		{	
		  			//If distance becomes to high, than victim is saved and predator no longer follow it
		  			if((x-Ax)*(x-Ax)+(y-Ay)*(y-Ay) > 300*300)focus = -1;
		  			else {
		  				//Continue chasing
		  				//Speed vector towards victim
		  				vx = -((int)(1.0*vmax*(x-Ax)/sqrt(1.0*(x-Ax)*(x-Ax)+(y-Ay)*(y-Ay))));
						vy = -((int)(1.0*vmax*(y-Ay)/sqrt(1.0*(x-Ax)*(x-Ax)+(y-Ay)*(y-Ay))));
						return;
					}
				
				}
			}
	  	
	  	
	  	fish::interaction(A); 
	}
	
	
};


//visualization if fush
void otricovka(fish value) 
{
	if(not value.life)return;
	txSetFillColor (value.color);
	txSetColor (value.color);
	txRectangle ((int)(value.x-2*value.size), (int)(value.y-value.size), (int)(value.x+2*value.size), (int)(value.y+value.size));
	int napravlenie;
	
	if(value.vx>0)napravlenie = 1;
	else napravlenie = -1;
	
	POINT star[3] = {{value.x+napravlenie*2*value.size,value.y+value.size}, {value.x+napravlenie*2*value.size,value.y-value.size}, {value.x+napravlenie*3.5*value.size,value.y}};
	txPolygon (star, 3);	
	POINT star2[3] = {{value.x-napravlenie*4*value.size,value.y+value.size}, {value.x-napravlenie*4*value.size,value.y-value.size}, {value.x-napravlenie*value.size,value.y}};
	txPolygon (star2, 3);
	txSetFillColor (RGB(0,0,0));
	txSetColor (RGB(0,0,0));
	POINT star3[3] = {{value.x+napravlenie*0.4*value.size,value.y+0.3*value.size}, {value.x+napravlenie*0.4*value.size,value.y-0.3*value.size}, {value.x+napravlenie*1.6*value.size,value.y}};
	txPolygon (star3, 3);
		
	POINT star4[4] = {{value.x-1*value.size,value.y-value.size}, {value.x+1*value.size,value.y-value.size}, {value.x+(1.0-napravlenie*0.3)*value.size,value.y-1.5*value.size}, {value.x-(1.0+napravlenie*0.3)*value.size,value.y-1.5*value.size}};
	txPolygon (star4, 4);
	txCircle (value.x+napravlenie*2.6*value.size, value.y-0.1*value.size, 0.2*value.size);   


}



 int   main(){  
 
	 // int i,j;
	 int x,y,n=0,nx,kolvo_vid;
	 printf("print size of window \n"); 
	 printf("X Axle: ");
	 scanf("%d",&x);
	 printf("Y Axle: ");
	 scanf("%d",&y);
	 printf("Print ammount of types of fishes: ");
	 scanf("%d",&kolvo_vid);

	 //how much fishes of each type, 2 4 1 2 5
	 // not all compilers work with this

	int vidi[1000];


	 for(int i=0;i<kolvo_vid;i++)
		{
			printf("Total %d fishes, Print ammont of fishes of %d type : ", n, i+1);
			scanf("%d",&vidi[i]);
			n+=vidi[i];	
		}
	 
	 printf("Print ammount of Predator fishes: ");
	 scanf("%d",&nx);
	 
	   // massive of nonPredator fishes
	   // not all compilers work
	//  fish TR[n];
	  fish TR[10000];
	  n = 0;
	  for(int i=0;i<kolvo_vid;i++)
	   {	
	    int size = rand()%15 + 2;
	    int vcr = rand()%10 + 2;
	    COLORREF color = RGB(10,10+rand()%240,10+rand()%240);
	  	for(int j=0;j<vidi[i];j++)
	  	{
	  		TR[n] =  fish(color,vcr,size,rand()%(x-20)+10,rand()%(y-20)+10,n);
	  		n++;
		  }
	  		 
	   }
	   // massive of Predator fishes
	    // not all compilers work
		//redatory_fish xishn[nx];
	   predatory_fish xishn[10000];	
	   for(int j=0;j<nx;j++)
	  	{
	  		xishn[j] =  predatory_fish(5,15,rand()%(x-20)+10,rand()%(y-20)+10,j);
		} 
		
		
	// before this point is inisialization / after this point is working cycle

	 txCreateWindow(x,y);
	 txSetFillColor (RGB(255,255,255));
	 txRectangle (0, 0, x, y); 
	 
	 
	 //work of aquarium untill pressed ESC
	 while(!txGetAsyncKeyState (VK_ESCAPE))
	{
	  txSetFillColor (RGB(255,255,255));
	  txRectangle (0, 0, x, y); 
	  txTextOut (x-200, y-30, "press ESC to quit");
	  

	  	 
	  // interaction between nonPredator fishes	 
	  
	  for(int i=0;i<n;i++)
	  	for(int j=i;j<n;j++)
	  	 {
	  	    if(i==j)continue;
	  	 	TR[i].interaction(&TR[j]);
		   }
		   
		   
		// interaction between Predator fishes	 
	  
	  for(int i=0;i<nx;i++)
	  	for(int j=i;j<nx;j++)
	  	 {
	  	    if(i==j)continue;
	  	 	xishn[i].interaction(&xishn[j]);
		   }
		   
	 // interaction between Predators and nonPredators, they deflect from nonPredators if their size +- same
	  for(int i=0;i<nx;i++)
	      for(int j=0;j<n;j++)
	  	 	xishn[i].interaction(&TR[j]);
		
	  
	  // interaction between fishes and walls
	  for(int i=0;i<n;i++)
	      TR[i].interaction(x,y);
	  for(int i=0;i<nx;i++)
	      xishn[i].fish::interaction(x,y);
	  
	 
	  
	  // visualization of fishes
	  
	  for(int i=0;i<n;i++)
	      otricovka(TR[i]);
	     
	  for(int i=0;i<nx;i++)
	    otricovka(xishn[i]);
	      
	  txSleep(50);
	 }
	return 0;
}


