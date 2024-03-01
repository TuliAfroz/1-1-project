#include"iGraphics.h"
#include<math.h>
#include<stdlib.h>
#include<string.h>


FILE *shop_file;
FILE *info_customer;
#define MAX_INVOICES 100
struct customers
{
	char customer[50];
	char phone_no[12];
	float due;
	float paid;
	char date[50];
};	
struct items
{
	char item[50];
	float price;
	int qty;
};

struct orders
{
	char customer[50];
	char date[50];
    int numOfItems;
	float paid;
	float due;
	struct items itm[50];
};

struct customers buyer;
struct orders ord;
int opt, n;

struct customers customer;
struct customers record1;
struct orders order;

float total = 0;
float paid;

char name[50];
struct orders record[MAX_INVOICES]; // Declare the array of orders
int numInvoices = 0;

//for home page
double left = 300, bottom = 700 - 150, dx = 300, dy = 50, x = 1000, y = 800;
int scene = -1;
int rx1 = x*0.35;
int ry1 = y*0.80;

//for scene 1 text input
char str[100], str2[100];
int len;
int mode;
int count; // for textbox input number

//for generate_Invoice of scene 1

int position; // position in itm array
int items_bought;

int toggle_generate = 0;
int color = 0;
//for scene 2
// found is initialized to values other than 0 or 1 because of conditions in functions
int found = 3;
int search_called = 0;



/*
	function iDraw() is called again and again by the system.

	*/
void drawTextBox(int x, int y)
{
	iSetColor(150, 150, 150);
	iRectangle(x, y, 400, 30);

}
void textbox(int x1, int y1, char str[])
{
	drawTextBox(x1, y1);
	if (mode == 1)
	{
		iSetColor(255, 255, 255);
		iRectangle(x1, y1, 400, 30);
		iSetColor(255, 255, 255);
		iText(x1, y1 + 30 * .5, str);
	}

	
}

void backMouse(int mx,int my){
	if (mx >= rx1 + 420 && mx <= rx1 +500 && my >= ry1-435 && my <= ry1-400)
			{
				scene = 0;
				total = 0;
				toggle_generate = 0;
				count = 0;
				items_bought = 0;
				search_called = 0;
				found = 3;
				total = 0;
			}
}
void back_button()
{
	iSetColor(255, 255, 20);
	iRectangle(rx1 + 420, ry1 - 435, 80, 35);
	iText(rx1 + 425, ry1 - 425, "Home", GLUT_BITMAP_TIMES_ROMAN_24);
}	

void home_page(){
	iShowBMP(rx1-50, ry1-175, "pictures\\halkhata.bmp");
    iSetColor(255, 255, 255);
	iText(rx1, ry1 - 500, "Press right arrow to go to menu.", GLUT_BITMAP_TIMES_ROMAN_24);
}

void menuMouse(int mx,int my){
	if (mx >= 400 && mx <= 700 && my >= 640 && my <= 690)
			{
				scene = 1;
			}
			
			 else if (mx >= 400 && mx <= 700 && my >= 480 && my <= 530)
			 {
			 	scene = 2;
				search_called = 0;
				found = 3;
			 }
		
			 else if (mx >= 400 && mx <= 700 && my >= 320 && my <= 370)
			 {
			 	scene = 3;
				search_called = 0;
				found = 3;
			 }
			
			else if (mx >= 400 && mx <= 700 && my >= 160 && my <= 210)
			{
				scene = 4;	
			}
		
}

void menu_page(){
	
	iSetColor(150, 30, 20);
	iRectangle(0, 0, x, y);
	iFilledRectangle(0, 0, x, y);
	int rx1 = x*0.40;
	int ry1 = y*0.50;
	iSetColor(255, 255, 255);
	iRectangle(rx1, ry1 + 80, dx, dy);
	iRectangle(rx1, ry1 + 240, dx, dy);
	iRectangle(rx1, ry1 - 80, dx, dy);
	iRectangle(rx1, ry1 - 240, dx, dy);
	// iSetColor(255, 255, 255);
	//menu
	iText(rx1+40, ry1+325, "HALKHATA", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(rx1 + 37, ry1 + 247, "1.Generate Invoice", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(rx1 + 37, ry1 + 87, "2.Search Invoices", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(rx1 + 37, ry1 - 73, "3.Find Customer", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(rx1 + 37, ry1 - 233, "4.Exit", GLUT_BITMAP_TIMES_ROMAN_24);
}
void InvoiceHeader(char name[50], char date[30])
{   

	char str_name[50];
	char str_date[30];
    sprintf(str_name, "%s", name);
	sprintf(str_date, "%s", date);
	iSetColor(255,255,255);
	iText(rx1 + 150, ry1 + 50, "abc shop", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(rx1, ry1 - 10, " -----------------", GLUT_BITMAP_HELVETICA_18);
	iText(rx1, ry1 - 20 * 1.5, "DATE:", GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 75, ry1 - 20 * 1.5, date, GLUT_BITMAP_HELVETICA_18);
	iText(rx1, ry1 - 30 * 1.5, "Invoice to:", GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 75 * 1.5, ry1 - 30 * 1.5, name, GLUT_BITMAP_HELVETICA_18);
	iText(rx1, ry1 - 40 * 1.5, "---------------------------------------", GLUT_BITMAP_HELVETICA_18);
	iText(rx1, ry1 - 50 * 1.5, "Items", GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 80 * 1.5, ry1 - 50 * 1.5, "Qty", GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 80 * 4.5, ry1 - 50 * 1.5, "Total", GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 80 * 3, ry1 - 50 * 1.5, "Unit Price", GLUT_BITMAP_HELVETICA_18);
	//iText(rx1, ry1 - 80*1.5, "---------------------------------------", GLUT_BITMAP_HELVETICA_18);

}
void InvoiceBody(char item[30], int qty, float price)
{    
	char str_item[100];
	char str_qty[100];
	char str_price[100];
	char str_uprice[100];

    sprintf(str_item,"%s",item);
	sprintf(str_qty, "%d", qty);
	sprintf(str_uprice, "%.2f",price);
	sprintf(str_price, "%.2f", qty*price);


	iText(rx1, ry1 - 60 * 1.5, str_item, GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 80 * 1.5, ry1 - 60 * 1.5, str_qty, GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 80 * 4.5, ry1 - 60 * 1.5, str_price, GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 80 * 3, ry1 - 60 * 1.5, str_uprice,GLUT_BITMAP_HELVETICA_18);
	printf("\n%s\t\t", item);
	printf("%d\t\t", qty);
	printf("%.2f\t\t", qty * price);
	printf("\n");
}
void generate_invoice()
{
	

	// input 
	ord.due = total - ord.paid;
	buyer.paid = ord.paid;
	buyer.due = ord.due;


	// saving bill
	shop_file = fopen("shop_bill.txt", "a+");

	fwrite(&ord, sizeof(struct orders), 1, shop_file);

	fclose(shop_file);

	 //saving customer record
	 info_customer = fopen("customer.txt", "a+");
	 fwrite(&buyer, sizeof(struct customers), 1, info_customer);

	 fclose(info_customer);

}
void InvoiceFooter(float total, float paid)
{
	


	char str_total[100];
	char str_bepaid[100];
	char str_due[100];
	char str_paid[100];

	sprintf(str_paid, "%f", paid);
	sprintf(str_total, "%f", total);
	sprintf(str_due, "%f", total - paid);
	sprintf(str_bepaid, "%f", paid - total);
	iText(rx1, ry1 - 255, "---------------------------------------", GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 40 * 2, ry1 - 90 * 3, "-------", GLUT_BITMAP_HELVETICA_18);
	iText(rx1, ry1 - 90 * 3, "Net Total", GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 40 * 6, ry1 - 90 * 3, str_total, GLUT_BITMAP_HELVETICA_18);
	iText(rx1, ry1 - 90 * 3 - 30, "Paid", GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 40 * 6, ry1 - 90 * 3 - 30, str_paid, GLUT_BITMAP_HELVETICA_18);
	iText(rx1 + 40 * 2, ry1 - 90 * 3 - 30, "-------", GLUT_BITMAP_HELVETICA_18);
	//iText(rx1 , ry1 - 110, "---------------------------------------", GLUT_BITMAP_HELVETICA_18);
	iText(rx1, ry1 - 315, "---------------------------------------", GLUT_BITMAP_HELVETICA_18);
	if (paid >= total)
	{
		iText(rx1, ry1 - 90 * 3 - 60, "Amount to be returned :", GLUT_BITMAP_HELVETICA_18);
		iText(rx1 + 40 * 6, ry1 - 90 * 3 - 60, str_bepaid, GLUT_BITMAP_HELVETICA_18);
	}
	else
	{
		iText(rx1, ry1 - 90 * 3 - 60, "Due Amount: ", GLUT_BITMAP_HELVETICA_18);

		iText(rx1 + 40 * 6, ry1 - 90 * 3 - 60, str_due, GLUT_BITMAP_HELVETICA_18);

	}
}
void invoice_mouse(int mx,int my){
	
			if (mx >= 348 && mx <= 748 && my >= 640 && my <= 690 && mode == 0)
			{
				mode = 1;
				
			}

			if (mx >= rx1 + 420 && mx <= rx1 + 500 && my >= ry1 && my <= ry1 + 35)
			{
				count++;
				color = 1;
				if (count == 1)
				{
					
					strcpy(ord.customer, str2);
					strcpy(buyer.customer, ord.customer);
				}

				if (count == 2)
				{
					strcpy(buyer.phone_no, str2);
					
				}

				strcpy(ord.date, __DATE__);
				strcpy(buyer.date, __DATE__);

				if (count == 3)
				{
					ord.numOfItems = atoi(str2);
					
				}

				if (position < ord.numOfItems)
				{
					if (count == 4)
					{
						strcpy(ord.itm[position].item, str2);
						
					}

					if (count == 5)
					{
						ord.itm[position].qty = atoi(str2);
						
					}
					if (count == 6)
					{
						
						ord.itm[position].price = atof(str2);
						total += ord.itm[position].qty * ord.itm[position].price;
						items_bought++;
						// count++;
						if (items_bought < ord.numOfItems)
						{
							position++;
							count = 3;
						}
					}

					if (count == 7)
					{
						
						ord.paid = atof(str2);
						buyer.paid = ord.paid;
						
						toggle_generate = 1;
						position = 0;
						generate_invoice();
						//count++;
					}
				}
			
}
}

void invoice_gen_page() 
{   
	    iSetColor(0, 255, 0);
		iRectangle(0, 0, x, y);

		iRectangle(rx1 + 420, ry1, 80, 35);
		textbox(rx1, ry1, str);
		
		iText(rx1 + 425, ry1 + 10, "  -->", GLUT_BITMAP_TIMES_ROMAN_24);
		back_button();
		if (color == 1)
		{
			iSetColor(135, 206, 235);
			iRectangle(rx1 + 420, ry1, 80, 35);

		}
		iText(rx1-50, ry1 - 500, "Click enter to save and Press --> to go to next page.", GLUT_BITMAP_TIMES_ROMAN_24);

		if (count == 0)
		{
			
			iText(rx1, ry1+40, " Please enter the name of the customer.", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		if (count == 1)
		{
			iText(rx1, ry1 +40, " Please enter the phone no of the customer.", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		if (count == 2)
		{
			iText(rx1, ry1+ 40, " Please enter the number of items.", GLUT_BITMAP_TIMES_ROMAN_24);
		}

		if (count == 3)
		{
			iText(rx1, ry1+ 40, " Please enter the name of the item.", GLUT_BITMAP_TIMES_ROMAN_24);
		}

		if (count == 4)
		{
			iText(rx1, ry1 + 40, " Please enter the quantity.", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		if (count == 5)
		{
			iText(rx1, ry1 + 40, " Please enter the unit price.", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		if (count == 6)
		{
			iText(rx1, ry1 + 40, " Enter Paid Amount.", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		if (count == 7)
		{
			iSetColor(149, 33, 246);
			iRectangle(0, 0, x, y);
			iText(rx1 + 425, ry1 + 10, " -->", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(rx1, ry1 + 40, " Paid. Press -->.", GLUT_BITMAP_TIMES_ROMAN_24);
			iSetColor(0, 0, 0);
			iRectangle(rx1,ry1, 400, 30);

		}

		if (count == 8)
		{
			int ry1_store = ry1;
			iSetColor(0, 0, 0);
			iFilledRectangle(0, 0, x, y);
			iSetColor(149, 33, 246);

			InvoiceHeader(ord.customer, ord.date);

			for (int i = 0; i < ord.numOfItems; i++)
			{
				InvoiceBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
				ry1 -= 30;
			}

			InvoiceFooter(total, ord.paid);
			ry1 = ry1_store;
			back_button();
			iText(rx1, ry1 - 500, "Press LEFT arrow to go to previuos page.", GLUT_BITMAP_TIMES_ROMAN_24);
		}

	
}

// void Search_invoice()

// {
// 	//input
// 	shop_file = fopen("shop_bill.txt", "r");

// 	float tot = 0;
// 	found=0;
// 	while (fread(&order, sizeof(struct orders), 1, shop_file))
// 	{
// 		if (!strcmp(order.customer, name))
// 		{
// 			for (int i = 0; i < order.numOfItems; i++)
// 			{
				
// 				tot += order.itm[i].qty * order.itm[i].price;
// 			}
// 			found = 1;
// 			//break;
// 		}
// 		// else
// 		// {
// 		// 	found = 0;
// 		// }
		
// 	}


// 	total = tot;

// 	fclose(shop_file);
// 	if (!found){
// 		total=0;
// 	}

// }
void drawPaginationButtons() {
    // Draw pagination buttons
    iSetColor(149, 33, 246);
    iRectangle(rx1, ry1 + 80, 100, 35);
    iRectangle(rx1 + 110, ry1 + 80, 100, 35);

    // Previous page button
    iText(rx1 + 10, ry1 +90, "Previous", GLUT_BITMAP_TIMES_ROMAN_24);

    // Next page button
    iText(rx1 + 120, ry1 +90, "Next", GLUT_BITMAP_TIMES_ROMAN_24);
}
void Search_invoice()
{
    // Input
    shop_file = fopen("shop_bill.txt", "r");
    

    int found = 0; // Flag to check if any invoices were found for the given name
    
    // Reset all previous invoices in the record
    memset(&record, 0, sizeof(struct customers));

    int invoice_index = 0; // Index for storing invoices

    while (fread(&order, sizeof(struct orders), 1, shop_file))
    {
        if (!strcmp(order.customer, name))
        {
            // Copy the current invoice to the record
            memcpy(&record[invoice_index], &order, sizeof(struct orders));
            
            // Increment the index for storing invoices
            invoice_index++;
            
            found = 1; // Set found flag to indicate at least one invoice was found
        }
		else found =0;
    }

    fclose(shop_file);

   
}

void draw_invoice( char *customer, char *date,  items items[], int numOfItems, float due, float paid, int rx1, int &ry1) {
    iSetColor(0, 0, 0);
    iFilledRectangle(0, 0, x, y);
    iSetColor(149, 33, 246);

    // Draw invoice header
    InvoiceHeader(customer, date);

    // Draw invoice body
    for (int j = 0; j < numOfItems; j++) {
        InvoiceBody(items[j].item, items[j].qty, items[j].price);
        ry1 -= 30;
    }

    // Draw invoice footer
    InvoiceFooter(due + paid, paid);
}
// Function to count the total number of invoices
int countTotalInvoices() {
    int totalInvoices = 0;
    for (int i = 0; i < MAX_INVOICES; i++) {
        if (record[i].customer[0] != '\0') {
            totalInvoices++;
        }
    }
    return totalInvoices;
}
int min(int a, int b) {
    return a < b ? a : b;
}
// Global variables for pagination
int currentPage = 0;
const int invoicesPerPage = 1;  // Adjust this value based on your interface

void search_iv_page() {

	iSetColor(149, 33, 246);
    iRectangle(0, 0, x, y);

    if (search_called == 0)
    {
        iRectangle(rx1 + 420, ry1, 80, 35);
        textbox(rx1, ry1, str);
        iText(rx1, ry1 + 40, " Please enter the name of the customer.", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(rx1 + 425, ry1 + 10, " -->", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(rx1 - 50, ry1 - 500, "Click enter to save and press --> to go to next page.", GLUT_BITMAP_TIMES_ROMAN_24);
    }
	else{
		int invoice_count = 0;
    // Calculate the starting and ending index of the invoices to display on the current page
    int startIndex = currentPage * invoicesPerPage;
    int endIndex = min(startIndex + invoicesPerPage, MAX_INVOICES);

    // Draw the invoices for the current page
    for (int i = startIndex; i < endIndex; i++) {
        if (record[i].customer[0] != '\0') {
            int ry1_store = ry1;
            iSetColor(0, 0, 0);
            iFilledRectangle(0, 0, x, y);
            iSetColor(149, 33, 246);

            InvoiceHeader(record[i].customer, record[i].date);

            for (int j = 0; j < record[i].numOfItems; j++) {
                InvoiceBody(record[i].itm[j].item, record[i].itm[j].qty, record[i].itm[j].price);
                ry1 -= 30;
            }

            InvoiceFooter(record[i].due + record[i].paid, record[i].paid);
            ry1 = ry1_store;
			invoice_count++;
			 drawPaginationButtons();
        }
		 back_button();
   
    }
	if (invoice_count == 0) {
            // If no invoices were found, display a message
            iText(rx1 - 10, ry1, "Sorry, no invoices found for:", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(rx1 + 100 + 50 * 4, ry1, name, GLUT_BITMAP_TIMES_ROMAN_24);
			 back_button();
        }
    
	}
}



// Function to handle mouse click events for pagination buttons
void handlePaginationButtonClick(int mx, int my) {

        // Check if the click is within the previous page button
        if (mx >= rx1 && mx <= rx1 + 100 && my >= ry1 + 80 && my <= ry1 + 80+35) {
            if (currentPage > 0) {
                currentPage--; // Move to the previous page
                // Redraw the screen to display the previous page
                iClear();
                search_iv_page();
            }
        }
        // Check if the click is within the next page button
        else if (mx >= rx1 + 110 && mx <= rx1 + 210 && my >= ry1 + 80 && my <= ry1 + 80+35) {
            int totalPages = ceil((double)countTotalInvoices() / invoicesPerPage);
            if (currentPage < totalPages - 1) {
                currentPage++; // Move to the next page
                // Redraw the screen to display the next page
                iClear();
                search_iv_page();
            }
        }
    
}


void search_iv_mouse(int mx,int my){
	if (mx >= 400 && mx <= 700 && my >= 640 && my <= 690)
			{
				mode = 1;
				
			}
			if (mx >= rx1 + 420 && mx <= rx1 + 500 && my >= ry1 && my <= ry1 + 35)
			{
				search_called = 1;
				Search_invoice();
			}
}

void findcustomer()
{

	shop_file = fopen("customer.txt", "r");



	while (fread(&customer, sizeof(struct customers), 1, shop_file))
	{
		if (strcmp(customer.customer, name) == 0)
		{
			found = 1;

			// converting to string
			char str_total[100];
			char str_bpaid[100];
			char str_bdue[100];
			sprintf(str_bpaid, "%.2f", total);
			sprintf(str_bdue, "%.2f", customer.due);

			

			break;
		}
		else
		{
			found = 0;
		}

	}


	fclose(shop_file);


}


void find_c_page(){
	iSetColor(255, 255, 255);
	iRectangle(0, 0, x, y);

		if (search_called == 0)
		{
			iText(rx1 - 100, ry1 + 100, "=======SEARCH Customer Record=======", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(rx1, ry1 + 40, "Enter the Customer Name : ", GLUT_BITMAP_HELVETICA_18);
			iRectangle(rx1 + 420, ry1, 80, 35);
			textbox(rx1, ry1, str);
			
			iText(rx1 + 425, ry1 + 10, " -->", GLUT_BITMAP_TIMES_ROMAN_24);
			back_button();
			iText(rx1 - 50, ry1 - 500, "Click enter to save and press --> to go to next page.", GLUT_BITMAP_TIMES_ROMAN_24);
			
		}


		if (found == 0)
		{
			//iText(rx1 - 10, ry1, "Sorry no such customer exists.", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(rx1 + 80, ry1 + 2 * 30, " NOT FOUND ", GLUT_BITMAP_HELVETICA_18);
			back_button();
		}

		if (found == 1)
		{

			char str_total[100];
			char str_bpaid[100];
			char str_bdue[100];
			sprintf(str_bpaid, "%.2f",customer.paid);
			sprintf(str_bdue, "%.2f", customer.due);

			//iText(rx1 + 80, ry1 - 2 * 30,"Date            ", GLUT_BITMAP_HELVETICA_18);
			//iText(rx1 + 80 * 3, ry1 - 2 * 30, customer.date, GLUT_BITMAP_HELVETICA_18);
			iText(rx1 + 80, ry1 - 3 * 30,"Customer Name  ", GLUT_BITMAP_HELVETICA_18);
			iText(rx1 + 80 * 3, ry1 - 3 * 30, customer.customer, GLUT_BITMAP_HELVETICA_18);
			iText(rx1 + 80, ry1 - 4 * 30,"Phone NO        ", GLUT_BITMAP_HELVETICA_18);
			iText(rx1 + 80 * 3, ry1 - 4 * 30, customer.phone_no, GLUT_BITMAP_HELVETICA_18);
			iText(rx1 + 80, ry1 - 5 * 30,"Paid            ", GLUT_BITMAP_HELVETICA_18);
			iText(rx1 + 80 * 3, ry1 - 5 * 30, str_bpaid, GLUT_BITMAP_HELVETICA_18);

			if (customer.due > 0)
			{
				iText(rx1 + 80, ry1 - 6 * 30, "Due        ", GLUT_BITMAP_HELVETICA_18);
				iText(rx1 + 80 * 3, ry1 - 6 * 30, str_bdue, GLUT_BITMAP_HELVETICA_18);
			}
			else
			{
				iText(rx1 + 80, ry1 - 7 * 30, " No Due", GLUT_BITMAP_HELVETICA_18);
			}

			iText(rx1, ry1 - 8 * 30, "_______________________________________ ", GLUT_BITMAP_HELVETICA_18);
			back_button();
			iText(rx1, ry1 - 500, "Press LEFT arrow to go to previuos page.", GLUT_BITMAP_TIMES_ROMAN_24);
			
		}

}
void find_c_mouse(int mx,int my){
	if (mx >= 400 && mx <= 700 && my >= 640 && my <= 690)
			{
				mode = 1;
				
			}
			if (mx >= rx1 + 420 && mx <= rx1 + 500 && my >= ry1 && my <= ry1 + 35)
			{
				search_called = 1;
				findcustomer();
			}
}
void exit_page()
{
	
	exit(0);
}

void iDraw() {
	//place your drawing codes here
	iClear();
	if(scene==-1){
		home_page();
	}
	if(scene==0){
       menu_page();
	}
	if(scene==1) {
	   invoice_gen_page();
	}
	if(scene==2){
		search_iv_page();
	   }
	if(scene==3){
       find_c_page();
	}
	if(scene==4){
     	exit_page();
	}
}


/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	printf("mx = %d, my= %d\n",mx,my);
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
	 if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
       if(scene==0){
		menuMouse(mx,my);
	   }
	   if(scene==1){
		invoice_mouse(mx,my);
	}
	   if(scene==2){
		search_iv_mouse(mx,my);
		handlePaginationButtonClick(mx,my);
	   }
	   if(scene==3){
		find_c_mouse(mx,my);
	   }
	 if(scene != 0){
		backMouse(mx,my);
	 }
				
	}
			
}
	// 	//place your codes here
	
	//   if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
	
	//  }
		

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {
	if (key == '\x1b') {
		exit(0);
	}

	if (scene == 1)
	{

		int i;
		if (mode == 1)
		{
			if (key == '\r')
			{

				mode = 0;
				strcpy(str2, str);
				for (i = 0; i < len; i++)
					str[i] = 0;
				len = 0;
				strcpy(name, str2);
				
			}
			if(key != '\b' && key != '\r'){
                str[len] = key;
				len++;
			}
			else
			{   if(len>0){
				str[--len]='\0';
			}
			else len=0;
				
			}
		}
	}
	
	if (scene == 2)
	{
		name[0] = NULL;
		int i;
		if (mode == 1)
		{

			if (key == '\r')
			{

				mode = 0;
				strcpy(str2, str);

				for (i = 0; i < len; i++)
					str[i] = 0;
				len = 0;
				strcpy(name, str2);
				
			}
			if(key != '\b'){
                str[len] = key;
				len++;
			}
			else
			{   if(len>0){
				str[--len]='\0';
			}
			else len=0;
				
			}
		}
	}
	if (scene == 3)
	{

		int i;
		if (mode == 1)
		{
			if (key == '\r')
			{

				mode = 0;
				strcpy(str2, str);
				for (i = 0; i < len; i++)
					str[i] = 0;
				len = 0;
				strcpy(name, str2);
				
			}
			if(key != '\b'){
                str[len] = key;
				len++;
			}
			else
			{   if(len>0){
				str[--len]='\0';
			}
			else len=0;
				
			}
		}
	}
	//place your codes for other keys here
	
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {
     if (scene == -1)
	{
		if (key == GLUT_KEY_RIGHT){ scene = 0; }
	}
	if(scene>=0){
		if(key== GLUT_KEY_LEFT){
			scene--;
			total = 0;
			toggle_generate = 0;
			count = 0;
			items_bought = 0;
			search_called = 0;
			found = 3;
			total = 0;}
	}
	
}


int main() {
	//place your own initialization codes here.
	iInitialize(1000, 800, "HALKHATA");
	
	return 0;
}

