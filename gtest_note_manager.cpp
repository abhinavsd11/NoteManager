#include <gtest/gtest.h>
#include "main.cpp" // Ensure this points to your main.cpp file

class NoteManagerTest : public ::testing::Test {
protected:
    NoteManager manager;

    void SetUp() override {
        // Clear the notes.txt file for fresh test cases
        std::ofstream file("notes.txt", std::ios::trunc);
        file.close();
    }

    void TearDown() override {
        // Clear the notes.txt file after each test
        std::ofstream file("notes.txt", std::ios::trunc);
        file.close();
    }
};

TEST_F(NoteManagerTest, AddNote) {
    manager.addNote("Test Title", "Test Content");
    Note* note = manager.searchByTitle("Test Title");
    ASSERT_NE(note, nullptr);
    EXPECT_EQ(note->text, "Test Content");
}

TEST_F(NoteManagerTest, DisplayNotes) {
    manager.addNote("Title 1", "Content 1");
    manager.addNote("Title 2", "Content 2");

    testing::internal::CaptureStdout();
    manager.displayNotes();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Title: Title 1"), std::string::npos);
    EXPECT_NE(output.find("Text: Content 1"), std::string::npos);
    EXPECT_NE(output.find("Title: Title 2"), std::string::npos);
    EXPECT_NE(output.find("Text: Content 2"), std::string::npos);
}

TEST_F(NoteManagerTest, DeleteNote) {
    manager.addNote("Title to Delete", "Some Content");
    manager.deleteNoteByTitle("Title to Delete");
    Note* note = manager.searchByTitle("Title to Delete");
    ASSERT_EQ(note, nullptr);
}

TEST_F(NoteManagerTest, SearchByTitle) {
    manager.addNote("Unique Title", "Content here");
    Note* note = manager.searchByTitle("Unique Title");
    ASSERT_NE(note, nullptr);
    EXPECT_EQ(note->title, "Unique Title");
    EXPECT_EQ(note->text, "Content here");
}

TEST_F(NoteManagerTest, SearchByText) {
    manager.addNote("Title", "Find this specific content");
    auto results = manager.searchByText("specific content");
    ASSERT_FALSE(results.empty());
    EXPECT_EQ(results[0]->text, "Find this specific content");
}

TEST_F(NoteManagerTest, ModifyNote) {
    manager.addNote("Old Title", "Old Content");
    Note* note = manager.searchByTitle("Old Title");

    ASSERT_NE(note, nullptr);
    manager.modifyNoteTitle(note, "New Title");
    manager.modifyNoteText(note, "New Content");

    Note* modifiedNote = manager.searchByTitle("New Title");
    ASSERT_NE(modifiedNote, nullptr);
    EXPECT_EQ(modifiedNote->text, "New Content");
}
