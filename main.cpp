#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

// Note class to represent individual notes
class Note
{
public:
    std::string title;
    std::string text;

    Note(const std::string &t, const std::string &txt) : title(t), text(txt) {}
};

// NoteManager class to handle note operations
class NoteManager
{
private:
    std::vector<Note> notes;
    const std::string filename = "notes.txt";

    // Helper function to validate input
    bool isValid(const std::string &input)
    {
        for (char c : input)
        {
            if (!isalnum(c) && c != '.' && c != ',' && c != ' ')
            {
                return false;
            }
        }
        return true;
    }

    void saveToFile()
    {
        std::ofstream file(filename);
        for (const auto &note : notes)
        {
            file << note.title << "\n"
                 << note.text << "\n";
        }
    }

    void loadFromFile()
    {
        std::ifstream file(filename);
        std::string title, text;
        while (std::getline(file, title) && std::getline(file, text))
        {
            notes.emplace_back(title, text);
        }
    }

public:
    NoteManager()
    {
        loadFromFile();
    }

    ~NoteManager()
    {
        saveToFile();
    }

    void addNote(const std::string &title, const std::string &text)
    {
        if (!isValid(title) || !isValid(text))
        {
            throw std::invalid_argument("Invalid input! Only letters, numbers, spaces, '.', and ',' are allowed.");
        }
        notes.emplace_back(title, text);
        std::cout << "Note added successfully!\n";
    }

    void displayNotes() const
    {
        if (notes.empty())
        {
            std::cout << "No notes available.\n";
            return;
        }
        for (const auto &note : notes)
        {
            std::cout << "Title: " << note.title << "\nText: " << note.text << "\n\n";
        }
    }

    void deleteNoteByTitle(const std::string &title)
    {
        auto it = std::remove_if(notes.begin(), notes.end(),
                                 [&title](const Note &note)
                                 { return note.title == title; });

        if (it != notes.end())
        {
            notes.erase(it, notes.end());
            std::cout << "Note deleted successfully!\n";
        }
        else
        {
            throw std::runtime_error("Note with the given title not found.");
        }
    }

    Note *searchByTitle(const std::string &title)
    {
        for (auto &note : notes)
        {
            if (note.title == title)
            {
                return &note;
            }
        }
        return nullptr;
    }

    std::vector<Note *> searchByText(const std::string &text)
    {
        std::vector<Note *> results;
        for (auto &note : notes)
        {
            if (note.text.find(text) != std::string::npos)
            {
                results.push_back(&note);
            }
        }
        return results;
    }

    void modifyNoteTitle(Note *note, const std::string &newTitle)
    {
        if (!isValid(newTitle))
        {
            throw std::invalid_argument("Invalid title format.");
        }
        note->title = newTitle;
    }

    void modifyNoteText(Note *note, const std::string &newText)
    {
        if (!isValid(newText))
        {
            throw std::invalid_argument("Invalid text format.");
        }
        note->text = newText;
    }
};

void menu()
{
    std::cout << "Menu:\n";
    std::cout << "1. Add Note\n";
    std::cout << "2. Display Notes\n";
    std::cout << "3. Search Note by Title\n";
    std::cout << "4. Search Notes by Text\n";
    std::cout << "5. Delete Note by Title\n";
    std::cout << "6. Modify Note\n";
    std::cout << "7. Exit\n";
}

int main()
{
    NoteManager manager;
    int choice;

    do
    {
        menu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // Clear the input buffer

        try
        {
            if (choice == 1)
            {
                std::string title, text;
                std::cout << "Enter note title: ";
                std::getline(std::cin, title);
                std::cout << "Enter note text: ";
                std::getline(std::cin, text);
                manager.addNote(title, text);
            }
            else if (choice == 2)
            {
                manager.displayNotes();
            }
            else if (choice == 3)
            {
                std::string title;
                std::cout << "Enter title to search: ";
                std::getline(std::cin, title);
                Note *note = manager.searchByTitle(title);
                if (note)
                {
                    std::cout << "Note found!\nTitle: " << note->title << "\nText: " << note->text << "\n";
                }
                else
                {
                    std::cout << "No note found with the given title.\n";
                }
            }
            else if (choice == 4)
            {
                std::string text;
                std::cout << "Enter text to search: ";
                std::getline(std::cin, text);
                auto results = manager.searchByText(text);
                if (!results.empty())
                {
                    std::cout << "Notes containing the text:\n";
                    for (const auto &note : results)
                    {
                        std::cout << "Title: " << note->title << "\nText: " << note->text << "\n";
                    }
                }
                else
                {
                    std::cout << "No notes found containing the given text.\n";
                }
            }
            else if (choice == 5)
            {
                std::string title;
                std::cout << "Enter title to delete: ";
                std::getline(std::cin, title);
                manager.deleteNoteByTitle(title);
            }
            else if (choice == 6)
            {
                std::string title;
                std::cout << "Enter title of the note to modify: ";
                std::getline(std::cin, title);
                Note *note = manager.searchByTitle(title);
                if (note)
                {
                    int subChoice;
                    std::cout << "1. Modify Title\n2. Modify Text\nEnter your choice: ";
                    std::cin >> subChoice;
                    std::cin.ignore(); // Clear the input buffer
                    if (subChoice == 1)
                    {
                        std::string newTitle;
                        std::cout << "Enter new title: ";
                        std::getline(std::cin, newTitle);
                        manager.modifyNoteTitle(note, newTitle);
                    }
                    else if (subChoice == 2)
                    {
                        std::string newText;
                        std::cout << "Enter new text: ";
                        std::getline(std::cin, newText);
                        manager.modifyNoteText(note, newText);
                    }
                    else
                    {
                        std::cout << "Invalid choice.\n";
                    }
                }
                else
                {
                    std::cout << "No note found with the given title.\n";
                }
            }
            else if (choice == 7)
            {
                std::cout << "Exiting program.\n";
            }
            else
            {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << "\n";
        }

    } while (choice != 7);

    return 0;
}
