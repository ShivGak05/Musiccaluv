#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>   // For rand()
#include <ctime>     // For seeding rand()
using namespace std;

class mymusic {
public:
    string songname;
    string artist;
    string genre;
    mymusic* next;
    mymusic* prev;

    // Constructor
    mymusic(string songname, string artist, string genre) {
        this->songname = songname;
        this->artist = artist;
        this->genre = genre;
        this->next = this;
        this->prev = this;
    }
};

// Function to insert a new song at the beginning
void insertnewsong(mymusic*& head, string songname, string artist, string genre) {
    mymusic* newnode = new mymusic(songname, artist, genre);
    
    if (head == nullptr) {
        head = newnode;
        return;
    }

    mymusic* last = head->prev;
    last->next = newnode;
    newnode->prev = last;
    newnode->next = head;
    head->prev = newnode;
    head = newnode; // Update head to new song
}

// Function to delete a song by name
void deletesongbyname(mymusic*& head, string songname) {
    if (head == nullptr) {
        cout << "Sorry! Your playlist is empty!" << endl;
        return;
    }

    mymusic* temp = head;
    mymusic* toDelete = nullptr;

    do {
        if (temp->songname == songname) {
            toDelete = temp;
            break;
        }
        temp = temp->next;
    } while (temp != head);

    if (toDelete == nullptr) {
        cout << "Song not found!" << endl;
        return;
    }

    if (toDelete->next == toDelete) {
        delete toDelete;
        head = nullptr;
        return;
    }

    toDelete->prev->next = toDelete->next;
    toDelete->next->prev = toDelete->prev;

    if (toDelete == head) {
        head = toDelete->next;
    }

    delete toDelete;
    cout << "Song deleted successfully!" << endl;
}

// Function to play all songs
void playallsongs(mymusic* head) {
    if (head == nullptr) {
        cout << "Your playlist is empty!" << endl;
        return;
    }

    mymusic* temp = head;
    cout << " Your Playlist: " << endl;

    do {
        cout << temp->songname << " - " << temp->artist << " [" << temp->genre << "]" << endl;
        temp = temp->next;
    } while (temp != head);
}

// Function to shuffle and play songs randomly
void playrandomsongs(mymusic* head) {
    if (head == nullptr) {
        cout << "Your playlist is empty!" << endl;
        return;
    }

    vector<mymusic*> songs;
    mymusic* temp = head;
    
    do {
        songs.push_back(temp);
        temp = temp->next;
    } while (temp != head);

    srand(time(0)); // Seed random function

    cout << "Playing songs in shuffle mode: " << endl;
    for (int i = songs.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(songs[i], songs[j]);
    }

    for (mymusic* song : songs) {
        cout << song->songname << " - " << song->artist << " [" << song->genre << "]" << endl;
    }
}

// Function to search songs by artist or genre
void searchsong(mymusic* head, string keyword, string type) {
    if (head == nullptr) {
        cout << "Your playlist is empty!" << endl;
        return;
    }

    mymusic* temp = head;
    bool found = false;

    cout << "Searching for " << type << ": " << keyword << endl;
    do {
        if ((type == "artist" && temp->artist == keyword) || 
            (type == "genre" && temp->genre == keyword)) {
            cout << temp->songname << " - " << temp->artist << " [" << temp->genre << "]" << endl;
            found = true;
        }
        temp = temp->next;
    } while (temp != head);

    if (!found) {
        cout << "No songs found for " << type << ": " << keyword << endl;
    }
}

// Function to save playlist to a file
void saveplaylist(mymusic* head) {
    if (head == nullptr) {
        cout << "No songs to save!" << endl;
        return;
    }

    ofstream file("playlist.txt");
    mymusic* temp = head;
    
    do {
        file << temp->songname << "," << temp->artist << "," << temp->genre << endl;
        temp = temp->next;
    } while (temp != head);

    file.close();
    cout << "Playlist saved successfully!" << endl;
}

// Function to load playlist from a file
void loadplaylist(mymusic*& head) {
    ifstream file("playlist.txt");
    string songname, artist, genre;
    
    while (getline(file, songname, ',') && getline(file, artist, ',') && getline(file, genre)) {
        insertnewsong(head, songname, artist, genre);
    }

    file.close();
    cout << "Playlist loaded successfully!" << endl;
}

// Main function
int main() {
    mymusic* playlist = nullptr;

    // Load playlist from file
    loadplaylist(playlist);

    char ans = 'y';
    while (ans == 'y' || ans == 'Y') {
        cout << "\n Choose an option:" << endl;
        cout << "1. Insert new song" << endl;
        cout << "2. Remove a song" << endl;
        cout << "3. Play all songs" << endl;
        cout << "4. Shuffle play" << endl;
        cout << "5. Search song by artist" << endl;
        cout << "6. Search song by genre" << endl;
        cout << "7. Save playlist" << endl;
        cout << "8. Exit" << endl;

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string songname, artist, genre;
            cout << "Enter song name, artist, and genre: ";
            getline(cin, songname);
            getline(cin, artist);
            getline(cin, genre);
            insertnewsong(playlist, songname, artist, genre);
            cout << "🎵 Song added successfully!" << endl;
        } 
        else if (choice == 2) {
            string songname;
            cout << "Enter song name to delete: ";
            getline(cin, songname);
            deletesongbyname(playlist, songname);
        } 
        else if (choice == 3) {
            playallsongs(playlist);
        } 
        else if (choice == 4) {
            playrandomsongs(playlist);
        } 
        else if (choice == 5) {
            string artist;
            cout << "Enter artist name: ";
            getline(cin, artist);
            searchsong(playlist, artist, "artist");
        } 
        else if (choice == 6) {
            string genre;
            cout << "Enter genre: ";
            getline(cin, genre);
            searchsong(playlist, genre, "genre");
        } 
        else if (choice == 7) {
            saveplaylist(playlist);
        } 
        else if (choice == 8) {
            saveplaylist(playlist);
            cout << "👋 Exiting Musicca. Your playlist is saved!" << endl;
            break;
        } 
        else {
            cout << "Invalid choice! Try again." << endl;
        }

        cout << "Do you want to continue? (y/n) ";
        cin >> ans;
    }

    return 0;
}
