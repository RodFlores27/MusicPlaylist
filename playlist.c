/*
Author: Rodolfo P. Flores III
Date: Oct 29, 2023
Description: A program that implements a music playlist using linked lists that is sorted by playlist name. The program
must continue to run until the user picks exit.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define BUFFER_SIZE 100

typedef struct song_tag
{
    // Strings
    char title[20];
    char artist[20];
    char album[20];
    struct song_tag *nextSong;
} song;

typedef struct playlist_tag
{
    char name[50];
    int songCount;
    song *songHead;
    struct playlist_tag *next;
} playlist;

playlist *p_list_of_playlists = NULL; // head pointer to playlist linked list

void printMenu();

playlist *addPlaylist(playlist **p);

song *addSongToPlaylist(playlist *p);

void removeSongFromPlaylist(playlist *p);

playlist *viewPlaylist(playlist *p);

void viewAllData(playlist *p);

void loadData(playlist **p);

void saveData(playlist *p);

int main()
{
    loadData(&p_list_of_playlists);

    while (1)
    {
        printMenu();
        char buffer[BUFFER_SIZE];
        fgets(buffer, BUFFER_SIZE, stdin);

        switch (atoi(buffer))
        {
        case 1:
            addPlaylist(&p_list_of_playlists);
            break;

        case 2:
            addSongToPlaylist(p_list_of_playlists);
            break;

        case 3:
            removeSongFromPlaylist(p_list_of_playlists);
            break;

        case 4:
            viewPlaylist(p_list_of_playlists);
            break;

        case 5:
            viewAllData(p_list_of_playlists);
            break;

        case 6:
            printf("Thank you!\n");
            saveData(p_list_of_playlists);

            return 0;
        default:
            printf("Invalid input.\n");
        }
    }
}

void loadData(playlist **p)
{

    // (b)Load Data
    //         - You must check whether the file exists.If the file exists,
    //     check if the count is 0. If the count is not 0, read each of the data in the file and save it in the linked lists.

    // Open the file.
    FILE *fp = fopen("data.txt", "r");
    if (fp == NULL)
    {
        printf("No old data loaded.\n");
        return;
    }

    // Read the file line by line.
    char buffer[BUFFER_SIZE];
    // get number of playlists
    fgets(buffer, BUFFER_SIZE, fp);
    buffer[strlen(buffer) - 1] == '\0';
    int numOfPlaylists = atoi(buffer);

    if (numOfPlaylists != 0)
    {
        for (int i = 0; i < numOfPlaylists; i++)
        {
            playlist *playList = (playlist *)malloc(sizeof(playlist));

            // get playlist title
            fgets(buffer, BUFFER_SIZE, fp);
            buffer[strlen(buffer) - 1] = '\0'; // To cut the newline from the string
            strcpy(playList->name, buffer);

            // get playlist song count
            fgets(buffer, BUFFER_SIZE, fp);
            buffer[strlen(buffer) - 1] = '\0'; // To cut the newline from the string
            playList->songCount = atoi(buffer);

            // Only gets to utilize for loop if songCount>0
            for (int j = 0; j < playList->songCount; j++)
            {

                song *songNode = (song *)malloc(sizeof(song));

                fgets(buffer, BUFFER_SIZE, fp);    // Get line of song data
                buffer[strlen(buffer) - 1] = '\0'; // To cut the newline from the string
                char *token = strtok(buffer, " ");
                // Initialize song
                strcpy(songNode->title, token);
                token = strtok(NULL, " ");

                strcpy(songNode->artist, token);
                token = strtok(NULL, " ");

                strcpy(songNode->album, token);
                token = strtok(NULL, " ");

                songNode->nextSong = NULL;
                // End of initialization

                // Connect songs

                if (j == 0)
                {
                    playList->songHead = songNode;
                }
                else
                {
                    song *temp = playList->songHead;
                    // Traverse while last song is not found
                    while (temp->nextSong != NULL)
                    {
                        printf("%s\n", temp->artist);
                        temp = temp->nextSong;
                    }
                    temp->nextSong = songNode;
                }

                songNode = NULL;
                free(songNode);
            }

            // case for first playlist node
            if (i == 0)
            {
                *p = playList;
            }

            // connect playlists
            else
            {
                playlist *temp = *p;
                // Traverse while last song is not found
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = playList;
            }
            playList = NULL;
            free(playList);
        }
    }
    // Close the file.
    fclose(fp);
}

void saveData(playlist *p)
{
    // (a)Save Data
    //     - You must check whether there is a data to be saved.If there are none,
    //     simply print a prompt saying
    //     so.Also,
    //     save count as 0 so that you will be able to keep track if file is empty.- You can use whatever format of data you want as long as you will be able to load the data properly.

    // Open the file.
    FILE *fp = fopen("data.txt", "w");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    // No playlists to save.
    if (p == NULL)
    {
        printf("No data to save.\n");
        fprintf(fp, "0");
    }
    else
    {
        // Extract playList count
        playlist *pPlaylist = p;
        int i = 0;
        while (pPlaylist != NULL)
        {
            pPlaylist = pPlaylist->next;
            i++;
        }
        fprintf(fp, "%d\n", i);

        // Extract playlists data
        pPlaylist = p;
        while (pPlaylist != NULL)
        {
            fprintf(fp, "%s\n", pPlaylist->name);
            fprintf(fp, "%d\n", pPlaylist->songCount);

            song *sPtr = pPlaylist->songHead;
            while (sPtr != NULL)
            {
                fprintf(fp, "%s ", sPtr->title);
                fprintf(fp, "%s ", sPtr->artist);
                fprintf(fp, "%s \n", sPtr->album);
                sPtr = sPtr->nextSong;
            }

            pPlaylist = pPlaylist->next;
        }
    }
    fclose(fp);
}

void viewAllData(playlist *p)
{
    // Iteratate through all playlists
    for (playlist *temp = p; temp != NULL; temp = temp->next)
    {
        printf("PLAYLIST: %s\n", temp->name);
        printf("SONG COUNT: %d\n", temp->songCount);
        // Iterate through each song inside playlist
        song *pSong = temp->songHead;
        while (pSong != NULL)
        {
            printf("\tSONG TITLE : %s\n", pSong->title);
            printf("\tSONG ARTIST : %s\n", pSong->artist);
            printf("\tSONG ALBUM : %s\n\n", pSong->album);

            pSong = pSong->nextSong;
        }
    }
}

playlist *viewPlaylist(playlist *p)
{
    printf("THE PLAYLISTS AVAILABLE ARE:\n");
    int i = 1;
    for (playlist *temp = p; temp != NULL; temp = temp->next, i++)
    {
        printf("\t[%i]  %s\n", i, temp->name);
    }

    // Ask user for playlist name
    printf("Enter playlist name: ");
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strlen(buffer) - 1] = '\0'; // To cut the newline from the string

    // Check if the playlist name exists
    for (playlist *temp = p; temp != NULL; temp = temp->next)
    {
        if (strcmp(temp->name, buffer) == 0) // if the arguments are the same string
        {
            printf("SONG COUNT: %d\n", temp->songCount);

            // Iterate through each song inside playlist
            song *pSong = temp->songHead;
            while (pSong != NULL)
            {
                printf("\tSONG TITLE : %s\n", pSong->title);
                printf("\tSONG ARTIST : %s\n", pSong->artist);
                printf("\tSONG ALBUM : %s\n\n", pSong->album);

                pSong = pSong->nextSong;
            }
            return temp;
        }
    }
    printf("Invalid playlist!\n");
}

void removeSongFromPlaylist(playlist *p)
{
    if (p == NULL)
    {
        printf("No playlists exist yet.\n");
    }
    else
    {
        printf("THE PLAYLISTS AVAILABLE ARE:\n");
        int i = 1;
        for (playlist *temp = p; temp != NULL; temp = temp->next, i++)
        {
            printf("\t[%i]  %s\n", i, temp->name);
        }

        // Ask user for playlist name
        printf("Enter playlist name: ");
        char buffer[BUFFER_SIZE];
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer) - 1] = '\0'; // To cut the newline from the string

        // Check if the playlist name exists
        // song *songToAdd = (song *)malloc(sizeof(song));
        for (playlist *temp = p; temp != NULL; temp = temp->next, i++)
        {

            if (strcmp(temp->name, buffer) == 0) // if the arguments are the same string
            {
                if (temp->songHead == NULL)
                {
                    printf("Playlist is still empty.");
                    return;
                }
                // Display all songs
                printf("THE SONGS AVAILABLE ARE : \n");
                song *songPtr = temp->songHead;
                for (int j = 1; songPtr != NULL; songPtr = songPtr->nextSong, j++)
                {
                    printf("\t[%d] %s by %s\n", j, songPtr->title, songPtr->artist);
                }

                char title[20];
                char artist[20];

                // Initialize song
                printf("Enter song title to delete: ");
                fgets(buffer, BUFFER_SIZE, stdin);
                buffer[strlen(buffer) - 1] = '\0';
                strcpy(title, buffer);

                printf("Enter song artist to delete: ");
                fgets(buffer, BUFFER_SIZE, stdin);
                buffer[strlen(buffer) - 1] = '\0';
                strcpy(artist, buffer);

                // Remove song to playlist
                songPtr = temp->songHead;

                // Stop when song details match
                while (songPtr->nextSong != NULL && !(strcmp(songPtr->title, title) == 0 &&
                                                      strcmp(songPtr->artist, artist) == 0))
                {
                    songPtr = songPtr->nextSong;
                }
                // song details don't match with the last song
                if (!(strcmp(songPtr->title, title) == 0 &&
                      strcmp(songPtr->artist, artist) == 0))
                {
                    printf("Song not found.\n");
                }
                else
                {
                    temp->songHead = songPtr->nextSong;
                    free(songPtr); // Delete node
                    printf("Successfully deleted song from playlist.\n");
                    temp->songCount -= 1;
                }
                return;
            }
        }
        printf("Invalid playlist!\n");
    }
}

song *addSongToPlaylist(playlist *p)
{
    // (b) Add Song to Playlist
    // - Make sure to check if there a playlist to add to. Ask which playlist to add the song to. If the playlist
    // is found, create an instance of the song and ask for the details. Make sure to initialize the members
    // properly.

    if (p == NULL)
    {
        printf("No playlists exist yet.\n");
    }
    else
    {

        printf("THE PLAYLISTS AVAILABLE ARE:\n");
        int i = 1;
        for (playlist *temp = p; temp != NULL; temp = temp->next, i++)
        {
            printf("\t[%i]  %s\n", i, temp->name);
        }

        // Ask user for playlist name
        printf("Enter playlist name: ");
        char buffer[BUFFER_SIZE];
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer) - 1] = '\0'; // To cut the newline from the string

        // Check if the playlist name exists
        song *songToAdd = (song *)malloc(sizeof(song));
        for (playlist *temp = p; temp != NULL; temp = temp->next, i++)
        {
            if (strcmp(temp->name, buffer) == 0) // if the arguments are the same string
            {
                // Initialize song
                printf("Enter song title: ");
                fgets(buffer, BUFFER_SIZE, stdin);
                buffer[strlen(buffer) - 1] = '\0';
                strcpy(songToAdd->title, buffer);

                printf("Enter song artist: ");
                fgets(buffer, BUFFER_SIZE, stdin);
                buffer[strlen(buffer) - 1] = '\0';
                strcpy(songToAdd->artist, buffer);

                printf("Enter song album: ");
                fgets(buffer, BUFFER_SIZE, stdin);
                buffer[strlen(buffer) - 1] = '\0';
                strcpy(songToAdd->album, buffer);

                songToAdd->nextSong = NULL;

                printf("Successfully added song to playlist\n");

                // Add song to playlist
                temp->songCount += 1;
                song *songPtr = temp->songHead;
                // first node case
                if (temp->songHead == NULL)
                {
                    temp->songHead = songToAdd;
                }
                else
                {
                    while (songPtr->nextSong != NULL)
                    {
                        songPtr = songPtr->nextSong;
                    }
                    songPtr->nextSong = songToAdd;
                }
                songPtr = songToAdd; // Set songPtr to added song for return

                songToAdd = NULL;
                free(songToAdd);

                return songPtr;
            }
        }
        printf("Invalid playlist!\n");
    }
}

playlist *addPlaylist(playlist **p)
{
    playlist *playList = (playlist *)malloc(sizeof(playlist));

    // Ask user for playlist name
    printf("Enter playlist name: ");
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strlen(buffer) - 1] = '\0'; // To cut the newline from the string

    // Initialize playlist members
    strcpy(playList->name, buffer);
    playList->songCount = 0;
    playList->songHead = NULL;
    playList->next = NULL;

    // case: first node
    if ((*p) == NULL)
    {
        (*p) = playList;
    }
    // Connect the linked list
    else
    {
        playlist *temp = *p;
        // traversal; stop on endNode or match
        while (temp->next != NULL && !(strcmp(temp->name, buffer) == 0))
        {
            temp = temp->next;
        }

        // Playlist name is unavailable
        if (strcmp(temp->name, buffer) == 0)
        {
            printf("Playlist name already exists!\n");
        }
        else
        {
            temp->next = playList;
        }

        // TODO: upon adding the playlist, sort them by name
        // Use strcmp. If -1, then the left one is the first one. If 1 then, the right argument should be the first

        playList = NULL;
        free(playList);

        return playList;
    }
}

void printMenu()
{

    printf("======= MENU =======\n");
    printf("[1] Add Playlist\n");
    printf("[2] Add Song to Playlist\n");
    printf("[3] Remove Song from Playlist\n");
    printf("[4] View a Playlist\n");
    printf("[5] View All Data\n");
    printf("[6] Exit\n");
    printf("Enter choice: ");
}