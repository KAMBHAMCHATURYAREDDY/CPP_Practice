#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <sstream>

enum Genre { FICTION, NONFICTION, SCIENCE, HISTORY, TECHNOLOGY };

class Book {
private:
    int bookID; float rating; double price;
    char availabilityStatus; bool isReferenceOnly;
    std::string title; Genre genre;

public:
    Book(): bookID(0), rating(0), price(0),
            availabilityStatus('A'), isReferenceOnly(false),
            title("Untitled"), genre(FICTION) {}

    Book(int id, float r, double p, char s, bool ref, const std::string &t, Genre g)
    : bookID(id), rating(r), price(p),
      availabilityStatus(s), isReferenceOnly(ref), title(t), genre(g) {}

    ~Book(){ std::cout<<"[Destructor] Book "<<bookID<<" destroyed.\n"; }

    int getBookID()const{return bookID;} 
    float getRating()const{return rating;}
    double getPrice()const{return price;} 
    char getAvailabilityStatus()const{return availabilityStatus;}
    bool getIsReferenceOnly()const{return isReferenceOnly;} 
    std::string getTitle()const{return title;}
    Genre getGenre()const{return genre;}

    void setBookID(int v){bookID=v;} 
    void setRating(float r){rating=r;}
    void setPrice(double p){price=p;} 
    void setAvailabilityStatus(char s){availabilityStatus=s;}
    void setIsReferenceOnly(bool r){isReferenceOnly=r;} 
    void setTitle(const std::string&t){title=t;}
    void setGenre(Genre g){genre=g;}

    void updateAvailability(bool borrowed){ availabilityStatus=borrowed?'B':'A'; }

    void displayDetails()const{
        static const char* genreStr[]={"FICTION","NONFICTION","SCIENCE","HISTORY","TECHNOLOGY"};
        std::cout<<"BookID: "<<bookID<<std::endl
                 <<"Title: "<<title<<std::endl
                 <<"Price: "<<price<<std::endl
                 <<"Rating: "<<rating<<std::endl
                 <<"Status: "<<availabilityStatus<<std::endl
                 <<"Reference: "<<(isReferenceOnly?"Yes":"No")<<std::endl
                 <<"Genre: "<<genreStr[genre]<<std::endl
                 <<"--------------------------"<<std::endl;
    }
};

void markAsReference(Book &b){ 
    b.setIsReferenceOnly(true); 
    std::cout<<"Book marked as Reference Only"<<std::endl << std::endl; 
}

bool isAffordable(const Book&b,double budget){return b.getPrice()<=budget;}

void printFormattedBookList(const Book arr[],int n){
    std::cout<<std::left<<std::setw(6)<<"ID"<<std::setw(20)<<"Title"<<std::setw(8)<<"Price"
             <<std::setw(6)<<"Rate"<<std::setw(8)<<"Status"<<std::setw(10)<<"Ref"<<std::endl;
    for(int i=0;i<n;i++)
        std::cout<<std::setw(6)<<arr[i].getBookID()<<std::setw(20)<<arr[i].getTitle()
                 <<std::setw(8)<<arr[i].getPrice()<<std::setw(6)<<arr[i].getRating()
                 <<std::setw(8)<<arr[i].getAvailabilityStatus()
                 <<std::setw(10)<<(arr[i].getIsReferenceOnly()?"Yes":"No")<<"\n";
    std::cout << std::endl;
}

void clearInputBuffer(){std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');}

int main(){
    std::cout << "This program manages an array of 3 Book objects." << std::endl;
    std::cout << "Use the index (0,1,2) to select which book in the array you want to work with." << std::endl << std::endl;

    Book books[3]={ Book(101,4.5,299,'A',false,"C++ Primer",TECHNOLOGY),
                    Book(102,3.8,150,'B',true,"Design Patterns",SCIENCE),
                    Book() };

    bool run=true;
    while(run){
        std::cout<<"\n===== Library Menu ====="<<std::endl
                 <<"1. Add/Replace Book"<<std::endl
                 <<"2. Mark Reference Only"<<std::endl
                 <<"3. Check Affordability"<<std::endl
                 <<"4. Update Availability"<<std::endl
                 <<"5. Display Book Details"<<std::endl
                 <<"6. Display All Books"<<std::endl
                 <<"7. Exit"<<std::endl
                 <<"Choose option: ";
        int c; std::cin>>c;
        std::cout << std::endl; // newline after menu choice
        clearInputBuffer();

        switch(c){
            case 1:{ // Add/Replace Book
                int i;
                std::cout<<"Enter index (0-2) to select which book in the array you want to modify: ";
                std::cin>>i;
                std::cout << std::endl;
                clearInputBuffer();

                std::string line;
                std::cout<<"Enter details in one line (ID, Rating, Price, Status(A/B), Reference(true/false), Genre(0-4), Title):"<<std::endl;
                std::getline(std::cin, line);

                // Replace commas with spaces
                for (char &c : line) if (c == ',') c = ' ';

                std::istringstream iss(line);
                int id, g;
                float r;
                double p;
                std::string s, refStr, titlePart, title;
                bool ref;
                Genre genre;

                iss >> id >> r >> p >> s >> refStr >> g;

                while (iss >> titlePart){
                    if (!title.empty()) title += " ";
                    if (titlePart.front() == '"') titlePart.erase(0,1);
                    if (titlePart.back() == '"') titlePart.pop_back();
                    title += titlePart;
                }

                char status = s.front() == '\'' ? s[1] : s[0];
                ref = (refStr == "true");
                genre = static_cast<Genre>(g);

                books[i] = Book(id, r, p, status, ref, title, genre);
                std::cout<<"Book details updated!"<<std::endl << std::endl;
                break;
            }

            case 2:{ // Mark Reference Only
                int i;
                std::cout<<"Enter index (0-2) to select which book to mark as reference: ";
                std::cin>>i;
                std::cout << std::endl;
                markAsReference(books[i]);
                break;
            }

            case 3:{ // Check Affordability
                int i;
                double b;
                std::cout<<"Enter index (0-2) to select which book: ";
                std::cin>>i;
                std::cout << std::endl;
                std::cout<<"Enter budget: ";
                std::cin>>b;
                std::cout << std::endl;
                std::cout<<(isAffordable(books[i],b)?"Affordable":"Not Affordable")<<std::endl << std::endl;
                break;
            }

            case 4:{ // Update Availability
                int i, br;
                std::cout<<"Enter index (0-2) to select which book: ";
                std::cin>>i;
                std::cout << std::endl;
                std::cout<<"Is Borrowed? (1/0): ";
                std::cin>>br;
                std::cout << std::endl;
                books[i].updateAvailability(br==1);
                std::cout<<"Availability updated"<<std::endl << std::endl;
                break;
            }

            case 5:{ // Display Book Details
                int i;
                std::cout<<"Enter index (0-2) to display one book, or -1 for all: ";
                std::cin>>i;
                std::cout << std::endl;
                if(i==-1) for(auto&b:books) b.displayDetails();
                else books[i].displayDetails();
                break;
            }

            case 6: // Display All Books
                printFormattedBookList(books,3);
                break;

            case 7: // Exit
                run=false;
                break;

            default:
                std::cout<<"Invalid option. Try again."<<std::endl << std::endl;
                break;
        }
    }
    std::cout<<"Exiting Library System..."<<std::endl;
    return 0;
}
