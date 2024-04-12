#include <iostream>
#include <list>
#include <queue>
#include <algorithm>

using namespace std;

// Video class
class Video {
public:
    int videoID;
    string title;
    string genre;
    string production;
    int numOfCopies;
    string imageFilename;

    Video(int id, const string& t, const string& g, const string& p, int copies, const string& image)
        : videoID(id), title(t), genre(g), production(p), numOfCopies(copies), imageFilename(image) {
    }
};

// Customer class
class Customer {
public:
    int customerID;
    string name;
    string address;

    Customer(int id, const string& n, const string& a)
        : customerID(id), name(n), address(a) {
    }
};

// CustomerRent class
class CustomerRent {
public:
    int customerID;
    queue<int> rentedVideos;

    CustomerRent(int id)
        : customerID(id) {
    }
};

// Movie Rental class
class MovieRental {
private:
    list<Video*> videoList;
    list<Customer*> customerList;
    list<CustomerRent*> rentedVideosList;
    int nextVideoID;
    int nextCustomerID;

public:
    MovieRental() : nextVideoID(1), nextCustomerID(1) {
        // Create some initial videos and customers
        videoList.push_back(new Video(nextVideoID++, "The Shawshank Redemption", "Drama", "Castle Rock Entertainment", 5, "shawshank.jpg"));
        videoList.push_back(new Video(nextVideoID++, "The Godfather", "Crime", "Paramount Pictures", 3, "godfather.jpg"));
        videoList.push_back(new Video(nextVideoID++, "Pulp Fiction", "Crime", "Miramax Films", 4, "pulpfiction.jpg"));
        videoList.push_back(new Video(nextVideoID++, "The Dark Knight", "Action", "Warner Bros.", 2, "darkknight.jpg"));
        videoList.push_back(new Video(nextVideoID++, "Inception", "Sci-Fi", "Warner Bros.", 6, "inception.jpg"));

        customerList.push_back(new Customer(nextCustomerID++, "John Doe", "123 Main St"));
        customerList.push_back(new Customer(nextCustomerID++, "Jane Smith", "456 Elm St"));
    }

    ~MovieRental() {
        // Clean up allocated memory
        for (Video* video : videoList) {
            delete video;
        }
        for (Customer* customer : customerList) {
            delete customer;
        }
        for (CustomerRent* rentedVideos : rentedVideosList) {
            delete rentedVideos;
        }
    }

    // Main menu
    void mainMenu() {
        int mainMenuChoice;
        do {
            cout << "====== Main Menu ======" << endl;
            cout << "[1] New Video" << endl;
            cout << "[2] Rent a Video" << endl;
            cout << "[3] Return a Video" << endl;
            cout << "[4] Show Video Details" << endl;
            cout << "[5] Display all Videos" << endl;
            cout << "[6] Check Video Availability" << endl;
            cout << "[7] Customer Maintenance" << endl;
            cout << "[8] Exit Program" << endl;

            cout << "Enter your choice: ";
            cin >> mainMenuChoice;

            switch (mainMenuChoice) {
                case 1:
                    insertVideo();
                    break;
                case 2:
                    rentVideo();
                    break;
                case 3:
                    returnVideo();
                    break;
                case 4:
                    showVideoDetails();
                    break;
                case 5:
                    displayAllVideos();
                    break;
                case 6:
                    checkVideoAvailability();
                    break;
                case 7:
                    customerMaintenanceMenu();
                    break;
                case 8:
                    break;
                default:
                    cout << "Invalid choice. Try again!" << endl;
                    break;
            }

        } while (mainMenuChoice != 8);
    }

    // Insert a new video
    void insertVideo() {
        string title, genre, production, imageFilename;
        int numOfCopies;

        cin.ignore(); // Clear input buffer
        cout << "Enter the title of the video: ";
        getline(cin, title);
        cout << "Enter the genre of the video: ";
        getline(cin, genre);
        cout << "Enter the production of the video: ";
        getline(cin, production);
        cout << "Enter the number of copies available: ";
        cin >> numOfCopies;
        cout << "Enter the image filename: ";
        cin.ignore();
        getline(cin, imageFilename);

        videoList.push_back(new Video(nextVideoID++, title, genre, production, numOfCopies, imageFilename));
        cout << "Video added successfully!" << endl;
    }

    // Rent a video
    void rentVideo() {
        int customerID, videoID;
        cout << "Enter the customer ID: ";
        cin >> customerID;
        cout << "Enter the video ID: ";
        cin >> videoID;

        Video* video = findVideoByID(videoID);
        Customer* customer = findCustomerByID(customerID);

        if (video == nullptr) {
            cout << "Video not found!" << endl;
        } else if (customer == nullptr) {
            cout << "Customer not found!" << endl;
        } else if (video->numOfCopies <= 0) {
            cout << "Video is not available for rent!" << endl;
        } else {
            video->numOfCopies--;
            CustomerRent* rentedVideos = findRentedVideosByID(customerID);
            if (rentedVideos == nullptr) {
                rentedVideosList.push_back(new CustomerRent(customerID));
                rentedVideos = findRentedVideosByID(customerID);
            }
            rentedVideos->rentedVideos.push(videoID);
            cout << "Video rented successfully!" << endl;
        }
    }

    // Return a video
    void returnVideo() {
        int customerID, videoID;
        cout << "Enter the customer ID: ";
        cin >> customerID;
        cout << "Enter the video ID: ";
        cin >> videoID;

        Video* video = findVideoByID(videoID);
        CustomerRent* rentedVideos = findRentedVideosByID(customerID);

        if (video == nullptr) {
            cout << "Video not found!" << endl;
        } else if (rentedVideos == nullptr) {
            cout << "No rented videos found for this customer!" << endl;
        } else {
            video->numOfCopies++;
            queue<int>& rentedVideoIDs = rentedVideos->rentedVideos;
            rentedVideoIDs = removeQueueElement(rentedVideoIDs, videoID);
            cout << "Video returned successfully!" << endl;
        }
    }

    // Show the details of a particular video
    void showVideoDetails() {
        int videoID;
        cout << "Enter the video ID: ";
        cin >> videoID;

        Video* video = findVideoByID(videoID);

        if (video == nullptr) {
            cout << "Video not found!" << endl;
        } else {
            cout << "Video ID: " << video->videoID << endl;
            cout << "Title: " << video->title << endl;
            cout << "Genre: " << video->genre << endl;
            cout << "Production: " << video->production << endl;
            cout << "Number of copies: " << video->numOfCopies << endl;
            cout << "Image filename: " << video->imageFilename << endl;
        }
    }

    // Display all videos in the store
    void displayAllVideos() {
        if (videoList.empty()) {
            cout << "No videos available!" << endl;
            return;
        }

        for (Video* video : videoList) {
            cout << "Video ID: " << video->videoID << endl;
            cout << "Title: " << video->title << endl;
            cout << "Genre: " << video->genre << endl;
            cout << "Production: " << video->production << endl;
            cout << "Number of copies: " << video->numOfCopies << endl;
            cout << "Image filename: " << video->imageFilename << endl;
            cout << "=========================" << endl;
        }
    }

    // Check whether a particular video is in the store
    void checkVideoAvailability() {
        int videoID;
        cout << "Enter the video ID: ";
        cin >> videoID;

        Video* video = findVideoByID(videoID);

        if (video == nullptr) {
            cout << "Video not found!" << endl;
        } else {
            cout << "Video availability: " << (video->numOfCopies > 0 ? "Available" : "Not Available") << endl;
        }
    }

    // Customer maintenance menu
    void customerMaintenanceMenu() {
        int customerMaintenanceChoice;
        do {
            cout << "=== Customer Maintenance ===" << endl;
            cout << "[1] Add New Customer" << endl;
            cout << "[2] Show Customer Details" << endl;
            cout << "[3] List of Videos Rented by a Customer" << endl;
            cout << "[4] Go back to Main Menu" << endl;
            cout << "Enter your choice: ";
            cin >> customerMaintenanceChoice;

            switch (customerMaintenanceChoice) {
                case 1:
                    addNewCustomer();
                    break;
                case 2:
                    showCustomerDetails();
                    break;
                case 3:
                    listRentedVideos();
                    break;
                case 4:
                    break;
                default:
                    cout << "Invalid choice. Try again!" << endl;
                    break;
            }
        } while (customerMaintenanceChoice != 4);
    }

    // Add new customer
    void addNewCustomer() {
        string name, address;

        cin.ignore(); // Clear input buffer
        cout << "Enter the name of the customer: ";
        getline(cin, name);
        cout << "Enter the address of the customer: ";
        getline(cin, address);

        customerList.push_back(new Customer(nextCustomerID++, name, address));
        cout << "Customer added successfully!" << endl;
    }

    // Show customer details
    void showCustomerDetails() {
        int customerID;
        cout << "Enter the customer ID: ";
        cin >> customerID;

        Customer* customer = findCustomerByID(customerID);

        if (customer == nullptr) {
            cout << "Customer not found!" << endl;
        } else {
            cout << "Customer ID: " << customer->customerID << endl;
            cout << "Name: " << customer->name << endl;
            cout << "Address: " << customer->address << endl;
        }
    }

    // List of videos rented by a customer
    void listRentedVideos() {
        int customerID;
        cout << "Enter the customer ID: ";
        cin >> customerID;

        CustomerRent* rentedVideos = findRentedVideosByID(customerID);

        if (rentedVideos == nullptr) {
            cout << "No rented videos found for this customer!" << endl;
        } else {
            cout << "Videos rented by customer ID " << rentedVideos->customerID << ":" << endl;
            queue<int> rentedVideoIDs = rentedVideos->rentedVideos;
            while (!rentedVideoIDs.empty()) {
                int videoID = rentedVideoIDs.front();
                rentedVideoIDs.pop();
                Video* video = findVideoByID(videoID);
                if (video != nullptr) {
                    cout << "Video ID: " << video->videoID << ", Title: " << video->title << endl;
                }
            }
        }
    }

    // Find a video by ID
    Video* findVideoByID(int videoID) {
        auto it = find_if(videoList.begin(), videoList.end(), [videoID](const Video* v) {
            return v->videoID == videoID;
        });
        if (it != videoList.end()) {
            return *it;
        }
        return nullptr;
    }

    // Find a customer by ID
    Customer* findCustomerByID(int customerID) {
        auto it = find_if(customerList.begin(), customerList.end(), [customerID](const Customer* c) {
            return c->customerID == customerID;
        });
        if (it != customerList.end()) {
            return *it;
        }
        return nullptr;
    }

    // Find rented videos by customer ID
    CustomerRent* findRentedVideosByID(int customerID) {
        auto it = find_if(rentedVideosList.begin(), rentedVideosList.end(), [customerID](const CustomerRent* r) {
            return r->customerID == customerID;
        });
        if (it != rentedVideosList.end()) {
            return *it;
        }
        return nullptr;
    }

    // Remove an element from the queue
    queue<int> removeQueueElement(queue<int>& q, int value) {
        queue<int> newQueue;
        while (!q.empty()) {
            int front = q.front();
            q.pop();
            if (front != value) {
                newQueue.push(front);
            }
        }
        return newQueue;
    }
};

int main() {
    MovieRental movieRental;
    movieRental.mainMenu();

    return 0;
}
