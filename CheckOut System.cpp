#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;


string WidgetName[30]; //name of widget
string ProductCode[30]; //unique product code
double ProductCost[30]; //cost of product
string DiscountTerms[30];  //number of items needed to qualify for discount
int NumOfProducts = 0; //our total number of prodcuts in our database
const int MaxCartQuantity = 100; //set constant int to be our max quantity for our cart so it doesn't crash our system
string CartContents[MaxCartQuantity];
int TotalCartItems = 0;
double Subtotal = 0;

void CompleteCheckout() //this function does our shipping cost caluclations, and applies our discounts
{
	double ShippingCost = 0;
	int TotalItemsOfProductCode = 0;
	int TotalItemsAvForDiscount = 0;
	double TotalDiscountCost = 0;
	int TotalItemsAtRegCost = 0;
	double TotalRegCost = 0;
	double TotalCost = 0;
	string CodeToSearchFor;

	for (int i = 0; i < NumOfProducts; i++)
	{
		TotalItemsOfProductCode = 0;
		TotalItemsAvForDiscount = 0;
		TotalDiscountCost = 0;
		TotalItemsAtRegCost = 0;
		TotalRegCost = 0;
		CodeToSearchFor = ProductCode[i];
		for (int j = 0; j < MaxCartQuantity; j++)
		{
			if (CartContents[j] == CodeToSearchFor)
			{
				TotalItemsOfProductCode++;
			}
		}
		if (DiscountTerms[i] == ",BOGOHP")
		{
			TotalItemsAvForDiscount = TotalItemsOfProductCode / 2;
			TotalDiscountCost = TotalItemsAvForDiscount * (ProductCost[i] * .5);
			TotalDiscountCost = (int)(100 * TotalDiscountCost) / 100.0;
			cout << "Total cost with discounts is $" << setprecision(2) << TotalDiscountCost << endl;
		}
		TotalItemsAtRegCost = TotalItemsOfProductCode - TotalItemsAvForDiscount;
		TotalRegCost = TotalItemsAtRegCost * ProductCost[i];
		TotalCost = TotalCost + TotalDiscountCost + TotalRegCost;
	}
		
	if (TotalCost < 50) // this else if statement runs through our shipping cost brackets
	{
		ShippingCost = 4.95;
	}
	else if (TotalCost >= 50 && TotalCost < 90)
	{
		ShippingCost = 2.95;
	}
	else
	{
		ShippingCost = 0;
	}

	TotalCost = TotalCost + ShippingCost;
	
	cout  << "Your total cost is $" << fixed << setprecision(2) << TotalCost;
}
void InitializeArrays()  //this fucntion is responsible for grabbing our product database from our csv and filling our arrays with our product information
{
	ifstream Products; //gives our input file an easily callable name
	Products.open("Assessment.csv", ios::in); //opens file for reading

	string line; //will be used to read lines 
	int linecount = 0;
	getline(Products, line); //ignore the header

	while (getline(Products, line) && !line.empty()) //loop will continue until we return an empty line
	{
		stringstream ProductStream(line);

		string temp;
		getline(ProductStream, temp, ',');
		WidgetName[linecount] = temp;
		cout << WidgetName[linecount] << " ";
		getline(ProductStream, temp, ',');
		ProductCode[linecount] = temp;
		cout << ProductCode[linecount] << " ";
		ProductStream >> ProductCost[linecount];
		getline(ProductStream, temp, ' ');
		DiscountTerms[linecount] = temp;
		cout << ProductCost[linecount] << " ";
		cout << DiscountTerms[linecount] << " " << endl;
		linecount++;
	}
	Products.close();
	NumOfProducts = linecount;
}

void AddProductsToCart(int ProductChoice, int ProductQuantity) //this function will add our products to our cart
{
	int StartingIndex = 0;
	cout << "Adding " << ProductQuantity << " of product code " << ProductCode[ProductChoice - 1] << " to cart." << endl;
	for (int i = 0; i < MaxCartQuantity; ++i)
	{
		if (CartContents[i] == "")
		{
			StartingIndex = i;
			break;
		}
	}
	for (int i = 0; i < ProductQuantity; ++i)
	{
		CartContents[StartingIndex + i] = ProductCode[ProductChoice - 1];
		cout << "Adding product at index " << StartingIndex + i << endl;
		TotalCartItems++;
	}
	Subtotal = (Subtotal + (ProductCost[ProductChoice - 1] * ProductQuantity));
	cout << "Your current subtotal is $" << Subtotal << "." << endl;
}
void PurchaseMenu(int NumOfProducts) //this function presents our purhcase interface to customer
{
	int ProductChoice = 0;
	int ProductQuantity = 0;
	bool KeepShopping = false;
	char selection = 'y';
	while ((selection == 'y'))
	{
		cout << "Please enter the number of the widget you would like to pruchase: " << endl;
		for (int i = 0; i < NumOfProducts; ++i)
		{
			cout << i + 1 << ".  " << WidgetName[i] << endl;
		}
		cin >> ProductChoice;
			while (ProductChoice < 0 || ProductChoice > NumOfProducts || !cin.good())
			{
				cout << "You have entered an iccorrect choice. Please try again." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cin >> ProductChoice;
			}
			cout << "Please enter the quantity you would like." << endl;
			{
				cin >> ProductQuantity;
				while (ProductQuantity > MaxCartQuantity || !cin.good() || TotalCartItems + ProductQuantity > MaxCartQuantity)
				{
					cout << "You have either entered an incorrect value, or are trying to add to many items to cart. Please try again." << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin >> ProductQuantity;
				}
				AddProductsToCart(ProductChoice, ProductQuantity);
			}
			cout << "Would you like to keep shopping Y or N?" << endl;
			cin >> selection;
			selection = (tolower(selection));
			while (selection != 'y' && selection != 'n' || !cin.good())
			{
				cout << "You have entered an incorrect value. Please try again." << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cin >> selection;
			}
	}
	CompleteCheckout();
}




int main()
{
	InitializeArrays();
	PurchaseMenu(NumOfProducts);
}
