#include<graphics.h>
#include<fstream.h>
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<string.h>
#include<math.h>

union REGS in, out;

struct Obj {
	int z;
	int x1, y1, x, y;
	int P[30];
	Obj() {
		z = x1 = y1 = x = y = 0;
		}
	};

int callmouse() {
	in.x.ax=1;
	int86(51, &in, &out);
	return 1; }

void mouseposi(int &xpos, int &ypos, int &click) {
	in.x.ax=3;
	int86(51, &in, &out);
	click = out.x.bx;
	xpos=out.x.cx;
	ypos=out.x.dx; }

int mousehide() {
	in.x.ax=2;
	int86(51, &in, &out);
	return 1; }

void setposi(int &xpos, int &ypos) {
	in.x.ax=4;
	in.x.cx=xpos;
	in.x.dx=ypos;
	int86(51, &in, &out); }

void mtrav(int &x, int &y, int &cl, int z) {
	while(!cl) {
	mouseposi(x,y,cl);

	setcolor(RED);
		gotoxy(1, 2);
		printf("\nmouse position is: %d, %d", x, y);
		printf("\nclick %d", cl);
		//printf("\nz is %d", z);

	 }
}

void undo(fstream &f, Obj &O) {
	int m = f.tellg() - sizeof(O);
		fstream g("tmp2.dat", ios::binary| ios::in| ios::out);

		f.seekg(0);

		while(f.tellg() < m) {
			f.read((char*)&O, sizeof(O));
			g.write((char*)&O, sizeof(O));
			}

		f.close(); g.close();
		remove("tmp.dat");
		rename("tmp2.dat", "tmp.dat");

		f.open("tmp.dat", ios::binary| ios::in| ios::out);
	}

void drawgraph(int &mx, int &my) {
	for(int i=0; i<mx; i+=mx/15) {
		setlinestyle(0, 1, 500);
		setcolor(4);
		line(i, 0, i, my-64);
		for(int j=i+mx/150; j<i+mx/15; j+=mx/150) {
			setcolor(3);
			setlinestyle(1, 1, 5);
			line(j, 0, j, my-64);
			}
		}
	for(i=0; i<my-64; i+=my/15) {
		setlinestyle(0, 1, 500);
		setcolor(4);
		line(0, i,mx ,i);
		for(int j=i+my/150; j<i+my/15; j+=my/150) {
			setcolor(2);
			setlinestyle(1, 1, 5);
			line(0, j, mx, j);
			}
		}
	}

inline void draw(int &mx, int &my)  {
	/* draw the buttons */
	bar(mx-100, my-35, mx-10,my-10);
		outtextxy(mx-70, my-30, "Circle");
	bar(mx-200, my-35, mx-110,my-10);
		outtextxy(mx-175, my-30, "Ellipse");
	bar(mx-300, my-35, mx-210,my-10);
		outtextxy(mx-290, my-30, "Rectangle");
	bar(mx-400, my-35, mx-310,my-10);
		outtextxy(mx-370, my-30, "Line");
	bar(mx-500, my-35, mx-410,my-10);
		outtextxy(mx-470, my-30, "UNDO");
	bar(mx-600, my-35, mx-510,my-10);
		outtextxy(mx-570, my-30, "Exit");
	bar(mx-100, my-65, mx-10,my-40);
		outtextxy(mx-90, my-60, "Open Graph");
	bar(mx-200, my-65, mx-110,my-40);
		outtextxy(mx-196, my-60, "Close Graph");
	bar(mx-300, my-65, mx-210,my-40);
		outtextxy(mx-290, my-60, "Polygon");
	bar(mx-400, my-65, mx-310,my-40);
		outtextxy(mx-390, my-60, "Free Hand");
	bar(mx-500, my-65, mx-410,my-40);
		outtextxy(mx-490, my-60, "Save File");
	bar(mx-600, my-65, mx-510,my-40);
		outtextxy(mx-590, my-60, "Load File");
	bar(mx-100, 40, mx-10,65);
		outtextxy(mx-90, 50, "HELP");
	bar(mx-200, 40, mx-110,65);
		outtextxy(mx-190, 50, "Colour");
	bar(mx-230, 40, mx-210,55);
		outtextxy(mx-222, 45, "+");
	setfillstyle(1,WHITE);
	bar(mx-600, my-90, mx-585,my-80);
	setfillstyle(1,RED);
	bar(mx-600, my-100, mx-585,my-90);
	setfillstyle(1,BLUE);
	bar(mx-600, my-110, mx-585,my-100);
	setfillstyle(1,GREEN);
	bar(mx-600, my-120, mx-585,my-110);
	setfillstyle(1,DARKGRAY);
	bar(mx-600, my-130, mx-585,my-120);
	setfillstyle(1,BROWN);
	bar(mx-600, my-140, mx-585,my-130);
	setfillstyle(1,YELLOW);
	bar(mx-600, my-150, mx-585,my-140);
	setfillstyle(1,MAGENTA);
	bar(mx-600, my-160, mx-585,my-150);
	setfillstyle(1,WHITE);
	}

void colour(int col) {
	if(col==15) 	setcolor(WHITE);
	if(col==16)	setcolor(RED);
	if(col==17)	setcolor(BLUE);
	if(col==18)	setcolor(GREEN);
	if(col==19)	setcolor(DARKGRAY);
	if(col==20)	setcolor(BROWN);
	if(col==21)	setcolor(YELLOW);
	if(col==22)	setcolor(MAGENTA);
}

void redo(fstream &f, Obj &O, int &mx, int &my) {
	cleardevice();
	setbkcolor(CYAN);
	draw(mx, my);

	f.seekg(0);
	f.read((char*)&O, sizeof(O));

	setcolor(WHITE);
	while(!f.eof()) {
		if(O.z==7) {
			mousehide();
			drawgraph(mx, my);
			callmouse();
			break; }
		f.read((char*)&O, sizeof(O));
		}
	f.clear();
	f.seekg(0);
	setcolor(WHITE);

	while(!f.eof()) {
		switch(O.z) {
			case 3:line(O.x1, O.y1, O.x, O.y);
				break;
			case 4:rectangle(O.x1, O.y1, O.x, O.y);
				break;
			case 5:ellipse(O.x1, O.y1,0, 360, O.x, O.y);
				break;
			case 6:circle(O.x1, O.y1, O.x);
				break;
			case 9:drawpoly(O.x, O.P);
				break;
			case 10:outtextxy(O.x, O.y, ".");
				break;
			case 14:setbkcolor(O.x);
				break;
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:colour(O.z);
				break;
			case 23:cleardevice();
				setbkcolor(3);
				setfillstyle(SOLID_FILL, 15);
				draw(mx, my);
				break;
			}
		f.read((char*)&O, sizeof(O));
		}
		int k = f.tellg();
		f.clear();
		f.seekg(k);
}

void intro() {
	int midx=300, midy = 200;
	setbkcolor(0);
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
	setusercharsize(1,2,3,4);

	for(int k = 1; k<=10 && kbhit() == 0; k++) {
	if(k != 4 && k!= 5) setfillstyle(k,WHITE +k);
		else setfillstyle(k-2, WHITE + k);
	for(int i = 0; i <= 50; i++) {
		fillellipse(midx, midy,100 +i,50 +i);
		delay(10); }
	setcolor(RED + k);
	switch(k) {
		case 1:
		case 7: outtextxy(midx - 25, midy, "WELCOME");
			break;
		case 2:
		case 8: outtextxy(midx , midy, "TO");
			break;
		case 3: outtextxy(midx - 85, midy + 10, "Paint to your hearts content");
		case 9: setcolor(WHITE);outtextxy(midx - 25, midy - 15, "MAD PAINTER");
			delay(1500);
			break;
		case 4: setcolor(YELLOW); outtextxy(midx - 25, midy, "PROEJCT");
			break;
		case 5: outtextxy(midx , midy, "BY");
			break;
		case 6: outtextxy(midx-25 , midy, "AKASH S");
			break;
		case 10:outtextxy(midx - 75, midy - 10, "THIS IS TOP SECRET.");
			outtextxy(midx - 140, midy + 10, "SO PUT YOUR NEXT WORDS IN COLOUR :>");
			break;
		}
	delay(500);
	}
	delay(1500);
	cleardevice();
	setbkcolor(BLACK);
	setcolor(14);
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 7);
	setusercharsize(3,1,3,1);
	outtextxy(220, 100, "MAD");
	outtextxy(130, 270, "PAINTER");
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	setusercharsize(2,1,3,2);
	setcolor(15);
	outtextxy(220, 400, "Akash S");
	setcolor(11);
	outtextxy(245, 415, "12 B");
	getch();
	cleardevice();

	setfillstyle(1, WHITE);
}

int main() {

	/* request auto detection */
	int gdriver = DETECT, gmode, errorcode;
	int mx, my, i;

	/* initialize graphics and local variables */
	initgraph(&gdriver, &gmode, "C:\\turboc3\\bgi");

	/* read result of initialization */
	errorcode = graphresult();
	if (errorcode != grOk)  /* an error occurred */
	{
		printf("Graphics error: %s\n", grapherrormsg(errorcode));
		printf("Press any key to halt:");
		getch();
		return 1; /* terminate with an error code */
	}

	mx=getmaxx();
	my=getmaxy();

	setbkcolor(2);

	i=SOLID_FILL;
	setfillstyle(i, 15);

	remove("tmp.dat");
	fstream f("tmp.dat", ios::binary| ios::in| ios::out);
	if (!f) { printf("tmp.dat not found!!!"); return 0; }

	intro();
	setbkcolor(3);

	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	setusercharsize(1,1,1,1);

	int a,b,col = 15;

	a=100; b=370;
	setposi(a,b);

	do{

	setcolor(RED);
	int x, y, cl, z=0;

	callmouse();

	draw(mx, my);

	do {
		mouseposi(x, y, cl);
		if(cl==1) {
			a=x; b=y;
			if(y > my-35 && y < my-10) {
				if(x > mx-600 && x < mx-510) z=1;
				if(x > mx-500 && x < mx-410) z=2;
				if(x > mx-400 && x < mx-310) z=3;
				if(x > mx-300 && x < mx-210) z=4;
				if(x > mx-200 && x < mx-110) z=5;
				if(x > mx-100 && x < mx-10)  z=6; }

			 else if(y > my-65 && y < my-40) {
				if(x > mx-600 && x < mx-510) z=12;
				if(x > mx-500 && x < mx-410) z=11;
				if(x > mx-400 && x < mx-310) z=10;
				if(x > mx-300 && x < mx-210) z=9;
				if(x > mx-200 && x < mx-110) z=8;
				if(x > mx-100 && x < mx-10)  z=7; }

			 else if(y > 40 && y < 65) {
				if(x > mx-300 && x < mx-210) z=23;
				if(x > mx-200 && x < mx-110) z=14;
				if(x > mx-100 && x < mx-10)  z=13;
				}

			 else if(x > my-600 && x < mx-585) {
				if(y > my-90 && y < my-80)   z=15;
				if(y > my-100 && y < my-90)  z=16;
				if(y > my-110 && y < my-100) z=17;
				if(y > my-120 && y < my-110) z=18;
				if(y > my-130 && y < my-120) z=19;
				if(y > my-140 && y < my-130) z=20;
				if(y > my-150 && y < my-140) z=21;
				if(y > my-160 && y < my-150) z=22; }

			 else z = cl = 0;
			 }

		setcolor(RED);

		gotoxy(1, 2);
		printf("\nmouse position is: %d, %d", x, y);
		printf("\nclick %d", cl);
		//printf("\nz is %d", z);
	}while(!cl);
	cl=0;
	int x1, y1;
	delay(500);
	setcolor(BLUE);

	Obj O;
	O.z=z;

	if(z==1) {
		return 1;
	}

	if(z==2) {
		undo(f, O);
		redo(f, O, mx, my);
	}

	if(z==3) {
		mousehide();
		outtextxy(mx-370, my-30, "Line");
		callmouse();

		mtrav(x, y, cl, z);
		x1 = x;    y1 = y;
		cl=0;
		delay(500);

		mtrav(x, y, cl, z);
		colour(col);
		line(x1, y1, x, y);

		O.x1 = x1;   O.y1 = y1;
		O.x = x;     O.y = y;

		f.write((char*)&O, sizeof(O));
		}

	if(z==4) {
		mousehide();
		outtextxy(mx-290, my-30, "Rectangle");
		callmouse();

		mtrav(x, y, cl, z);
		x1 = x;    y1 = y;
		cl=0;
		delay(500);

		mtrav(x, y, cl, z);

		mousehide();
		colour(col);
		rectangle(x1, y1, x, y);
		callmouse();

		O.x1 = x1;   O.y1 = y1;
		O.x = x;     O.y = y;

		f.write((char*)&O, sizeof(O));
		}

	if(z==5) {
		mousehide();
		outtextxy(mx-175, my-30, "Ellipse");
		callmouse();

		mtrav(x, y, cl, z);
		x1 = x;    y1 = y;
		cl=0;
		delay(500);

		mtrav(x, y, cl, z);
		int x2 = fabs(x-x1);
		cl=0;
		delay(500);

		mtrav(x, y, cl, z);
		int y2 = fabs(y1-y);

		mousehide();
		colour(col);
		ellipse(x1, y1,0,360, x2, y2);
		callmouse();

		O.x1 = x1;   O.y1 = y1;
		O.x = x2;     O.y = y2;

		f.write((char*)&O, sizeof(O));
		}

	if(z==6) {
		mousehide();
		outtextxy(mx-70, my-30, "Circle");
		callmouse();

		mtrav(x, y, cl, z);
		x1 = x;    y1 = y;
		cl=0;
		delay(500);

		mtrav(x, y, cl, z);
		int xr=sqrt(pow(x-x1,2) + pow(y-y1,2));

		mousehide();
		colour(col);
		circle(x1, y1, xr);
		callmouse();

		O.x1 = x1;   O.y1 = y1;
		O.x = xr;

		f.write((char*)&O, sizeof(O));
		}

	if(z==7) {
		mousehide();
		outtextxy(mx-90, my-60, "Open Graph");
		callmouse();
		delay(500);

		f.write((char*)&O, sizeof(O));
		redo(f, O, mx, my);
		}

	if(z==8) {
		mousehide();
		outtextxy(mx-196, my-60, "Close Graph");
		callmouse();

		int m = f.tellg();
		fstream g("tmp2.dat", ios::binary| ios::in| ios::out);

		f.seekg(0);

		while(f.tellg() < m) {
			f.read((char*)&O, sizeof(O));
			if(O.z != 7)
				g.write((char*)&O, sizeof(O));
			}

		f.close(); g.close();
		remove("tmp.dat");
		rename("tmp2.dat", "tmp.dat");

		fstream f("tmp.dat", ios::binary| ios::in| ios::out);
		if(!f) {
			cout<<"ERROR IN OPENING FILE. PLEASE RETRY LATER.";
			getch();
			return 0;}

		redo(f, O, mx, my);
	}

	if(z==9) {
		mousehide();
		outtextxy(mx-290, my-60, "Polygon");
		callmouse();
		delay(500);

		cleardevice();
		closegraph();

		int n;

		do {
		cout<<"Number of points: ";
		cin>>n;

		if(n>15 || n<3) {
			cout<<"PLEASE ENTER A VALID NUMBER OF POINTS BETWEEN 3 AND 15";
			getch(); }

		}while(n > 15 || n <3);

		initgraph(&gdriver, &gmode, "C:\\turboc3\\bgi");
		errorcode = graphresult();
		if (errorcode != grOk)  /* an error occurred */
		{
			printf("Graphics error: %s\n", grapherrormsg(errorcode));
			printf("Press any key to halt:");
			getch();
			return 1; /* terminate with an error code */
		}

		redo(f, O, mx, my);

		callmouse();

		int P[30];
		for(int i=0; i<n; i++) {
			mtrav(x, y, cl, z);
			cl=0;
			delay(500);
			P[2*i + 0]= x;
			P[2*i + 1]= y;
			}
		P[2*i + 0]= P[0];
		P[2*i + 1]= P[1];

		mousehide();
		colour(col);
		drawpoly(n+1, P);
		callmouse();

		O.x = n+1;
		for(i = 0; i<30; i++)  O.P[i] = P[i];
		f.write((char*)&O, sizeof(O));

		}

	if(z==10) {
		mousehide();
		outtextxy(mx-390, my-60, "Free Hand");
		callmouse();

		mtrav(x, y, cl, z);

		while(cl == 1) {
			colour(col);
			mouseposi(x,y,cl);
			mousehide();

			outtextxy(x-1, y-1, ".");
			callmouse();

			setcolor(RED);
			gotoxy(1, 2);
			printf("\nmouse position is: %d, %d", x, y);
			printf("\nclick %d", cl);
			//printf("\nz is %d", z);

			O.z = 10;
			int flag =0;
			f.seekg(0, ios::end);
			if ( f.tellg() ==0) {
			    printf("%d", O.x);
			    O.x = x; O.y = y;
			    flag =1;}

			else
			{  f.seekg(sizeof(O), ios::end);
			   f.read((char*)&O, sizeof(O));
				if((O.x != x && O.y != y)) {
				O.x = x; O.y = y;
				flag=1;
				}
			}
			if(flag==1)
			{
			       f.close();
			       f.open("tmp.dat",ios::app|ios::binary);
			       f.write((char*)&O, sizeof(O));
			       f.close();
			       f.open("tmp.dat",ios::in|ios::out|ios::binary);

			  }
	      }
	}


	if(z==11) {
		mousehide();
		outtextxy(mx-490, my-60, "Save File");
		callmouse();
		delay(500);

		cleardevice();
		closegraph();

		char S[15];

		do {
		clrscr();
		cout<<"Name of file: ";
		cin>>S;

		if(strlen(S) > 8) {
			cout<<"\nINVALID NAME. PLEASE ENTER ONLY MAXIMUM OF 8 CHARACTERS.";
			getch(); }

		}while(strlen(S) > 8);

		strcat(S, ".dat");

		fstream g(S, ios::binary| ios::in| ios::out| ios::trunc);
		if(!g) {
			cout<<"ERROR IN OPENING FILE. PLEASE RETRY LATER.";
			getch();
			return 0; }

		int k = f.tellg() - 1, i=0;

		f.clear(); f.seekg(0);
		f.read((char*)&O, sizeof(O));
		while(i<k) {
			g.write((char*)&O, sizeof(O));
			f.read((char*)&O, sizeof(O));
			i+=sizeof(O);
			}

		O.z = -1;
		g.write((char*)&O, sizeof(O));
		g.close();

		f.clear();
		f.seekg(k);

		initgraph(&gdriver, &gmode, "C:\\turboc3\\bgi");
		errorcode = graphresult();
		if (errorcode != grOk)  /* an error occurred */
		{
			printf("Graphics error: %s\n", grapherrormsg(errorcode));
			printf("Press any key to halt:");
			getch();
			return 1; /* terminate with an error code */
		}

		redo(f, O, mx, my);
		}

	if(z==12) {
		mousehide();
		outtextxy(mx-590, my-60, "Load File");
		callmouse();

		delay(500);

		cleardevice();
		closegraph();

		char S[14];
		cout<<"Name of file: ";
		cin>>S;

		strcat(S, ".dat");

		fstream g(S, ios::binary| ios::in| ios::out);
		if(!g) {
			cout<<"ERROR IN OPENING FILE. PLEASE RETRY LATER.";
			getch();
			return 0;}

		f.close(); remove("tmp.dat");
		f.open("tmp.dat", ios::binary| ios::in| ios::out);

		g.read((char*)&O, sizeof(O));
		while(O.z != -1) {
			f.write((char*)&O, sizeof(O));
			g.read((char*)&O, sizeof(O));
			}
		g.close();

		int k = f.tellg() - 1;
		f.clear();
		f.seekg(k);

		initgraph(&gdriver, &gmode, "C:\\turboc3\\bgi");
		errorcode = graphresult();
		if (errorcode != grOk)  /* an error occurred */
		{
			printf("Graphics error: %s\n", grapherrormsg(errorcode));
			printf("Press any key to halt:");
			getch();
			return 1; /* terminate with an error code */
		}

		redo(f, O, mx, my);
		}

	if(z==13) {
		mousehide();
		outtextxy(mx-90, 50, "HELP");
		callmouse();
		delay(500);

		cleardevice();
		closegraph();

		char K[50];

		ifstream h("projhelp.txt");
		if(!h) {
			cout<<"ERROR IN OPENING FILE. PLEASE RETRY LATER.";
			getch();
			return 0;}


		h.getline(K, 50, '\n');
		while(!h.eof()) {
			cout<<K<<"\n";
			h.getline(K, 50, '\n'); }
		cout<<"Press any key to exit.";
		getch();

		initgraph(&gdriver, &gmode, "C:\\turboc3\\bgi");
		errorcode = graphresult();
		if (errorcode != grOk)  /* an error occurred */
		{
			printf("Graphics error: %s\n", grapherrormsg(errorcode));
			printf("Press any key to halt:");
			getch();
			return 1; /* terminate with an error code */
		}

		redo(f, O, mx, my);
		}

	if(z==14) {
		mousehide();
		outtextxy(mx-190, 50, "Colour");
		callmouse();
		delay(500);

		cleardevice();
		closegraph();

		char col;

		do {
		clrscr();
		cout<<"\n\n\t\t\tMENU       "
		    <<"\nBLACK        ³  0  "
		    <<"\nBLUE         ³  1  "
		    <<"\nGREEN        ³  2  "
		    <<"\nCYAN         ³  3  "
		    <<"\nRED          ³  4  "
		    <<"\nMAGENTA      ³  5  "
		    <<"\nBROWN        ³  6  "
		    <<"\nLIGHTGRAY    ³  7  ";
		cout<<"\nChoose your colour: ";
		cin>>col;

		if(col > '7' || col < '0') {
			cout<<"\nPLEASE ENTER A VALID COLOUR NUMBER";
			getch(); }

		}while(col > '7' || col < '0');

		cout<<"\n Your change will be made when you press any key. ";
		getch();

		initgraph(&gdriver, &gmode, "C:\\turboc3\\bgi");
		redo(f, O, mx, my);
		setbkcolor(col-(int)'0');

		O.x = col - (int)'0';
		f.write((char*)&O, sizeof(O));
		f.close();
		}

	if(z>=15 && z<=22) {
		col  = z;
		f.write((char*)&O, sizeof(O));
		colour(col);
		}

	if(z==23) {
		outtextxy(mx-222, 45, "+");
		delay(500);
		cleardevice();
		setbkcolor(3);

		i=SOLID_FILL;
		setfillstyle(i, 15);

		setcolor(RED);
		callmouse();
		draw(mx, my);
		f.write((char*)&O, sizeof(O));
		}

	delete O.P;
	}while(!kbhit());

	cleardevice();
	closegraph();
	f.close();
	return 1;
}