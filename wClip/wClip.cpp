// wClip.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <WinUser.h>
#include <WinBase.h>
#include <cstdio>
constexpr auto BUF_SIZE = 1024;

int main(int argc, char** argv)
{
    char* content = static_cast<char*>(malloc(sizeof(char) * BUF_SIZE));
	size_t contentSize = 1;
    if (argc > 1)
    {
		size_t len = strlen(argv[1]);
		content = static_cast<char*>(realloc(content, len+1));
		memcpy(content, argv[1], len);
		content[len] = NULL;
		contentSize = len+1;
    }
    else
    {
	    char buffer[BUF_SIZE];
	    
	    if (content == nullptr)
	    {
	        perror("Failed to allocate content");
	        exit(1);
	    }
	    content[0] = '\0'; // make null-terminated
	    while (fgets(buffer, BUF_SIZE, stdin))
	    {
	        char* old = content;
	        contentSize += strlen(buffer);
	        content = static_cast<char*>(realloc(content, contentSize));
	        if (content == nullptr)
	        {
	            perror("Failed to reallocate content");
	            free(old);
	            exit(2);
	        }
	        strcat_s(content, contentSize, buffer);
	    }

	    if (ferror(stdin)) {
	        free(content);
	        return 2;
	    }
    }
	
	
    if (OpenClipboard(nullptr))
    {
        HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, contentSize * sizeof(char));
        LPVOID lockedPtr = GlobalLock(hglbCopy);
        memcpy(lockedPtr, content, contentSize);
        GlobalUnlock(hglbCopy);
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hglbCopy);
        CloseClipboard();
        printf("%s\n", content);
        printf("Copied %u bytes to clipboard \n", (unsigned) contentSize);
    }
    else
    {
        free(content);
        return 1;
    }
    free(content);
}
