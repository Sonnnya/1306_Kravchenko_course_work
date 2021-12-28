#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <ctype.h>
#include <errno.h>

/**To clear the console by using CLS**/
#if(defined(linux)||defined(__MACH__))
#define CLS system("clear")
#else
#define CLS system("cls")
#endif

#define MAXLEN 128/**Maximum string length**/


char *get_str(int max_len, int *len1);
/**Reads from the console a string no longer than max_len,
 the real length is written to len1. Returns the read string.**/

char *create_fname(char *fname, int *len);
/**Generates the file name fname with length len, which is entered by the user.
 If the user has entered an invalid character, displays a warning and requests
  fname again until it becomes correct.**/

int word_in_string(char *word,char *string);
/**Returns 1 if the word word is in the string string.
Otherwise, it returns 0.**/

int ok_file_name(char *fname, int len);
/**Returns 1 if the string fname of length len does not contain
 invalid characters. 0 in the opposite case. Also adds “.txt”
 to the fname at the end if its extension was not specified in
 the fname file name.**/

void write_in_file(char *file_name, char **array_of_strings, int len_of_array);
/**Writes an array of strings array_of_strings long len_of_array to a file named
file_name. If a file with the same name already existed, it starts "rewriting" the file.**/

int main()
{
    int i, slen, w_len, ns_len, infname_len, outfname_len;
    int n, count, k, option;
    char **str_arr = NULL;
    char *str1 = NULL;
    char *new_str = NULL;
    char *word = NULL;
    char *input_file_name = NULL;
    char *output_file_name = NULL;
    FILE *df;
    option = 1;
    k = 0;
    while(option == 1)
    {
    printf("Hello! This program replaces strings that contains a certain word with another\n");
    printf("specified string. Strings are the text that is written to the file.\n\n");

    printf("Please enter the file name:\n");
    if (k != 0) getchar(); /**Taking the empty input**/
    else k = 1;

    input_file_name = create_fname(input_file_name, &infname_len);

    printf("\nEnter the word:\n");
    word = get_str(MAXLEN, &w_len);
    printf("Enter the new string to replace the original ones with:\n");
    new_str = get_str(MAXLEN, &ns_len);

    df = fopen(input_file_name, "r");
    if(df != NULL)
    {
        str1 = (char*)malloc(MAXLEN*sizeof(char));
        if(str1 != NULL)
        {
            n = 0;
            count = 0;
            while(fgets(str1,MAXLEN,df)!=NULL) n++;

            rewind(df); /**Return the pointer to the beginning of the file**/
            str_arr=(char**)malloc(n*sizeof(char*));
            if(str_arr != NULL)
            {
                for(i = 0, count = 0; i < n; i++)
                {
                    str_arr[i] = (char*)malloc(MAXLEN*sizeof(char));
                    if(str_arr[i] != NULL)
                    {
                        count++;
                        fgets(str1,MAXLEN,df);
                        slen = strlen(str1);
                        str1[slen-1] = '\0'; /**Maximum string length**/
                        strcpy(str_arr[i], str1);
                    }
                    else
                    {
                        i = n;
                        puts("Error at string allocation! Array not completed!");
                    }
                }
                puts("------------------");
                for(i = 0; i < count; i++)
                {
                    if (word_in_string(word, str_arr[i])) strcpy(str_arr[i], new_str);
                }
                puts("Where do you want to write the result? Enter");
                puts("1 to rewrite original file");
                puts("2 to write in new file");
                puts("3 to do not write");

                scanf("%d", &option);
                do
                {
                    switch(option)
                    {
                    case 1:
                        write_in_file(input_file_name, str_arr, count);
                        break;
                    case 2:
                        puts("Please enter the name of new file");
                        getchar(); /**Taking the empty input**/

                        output_file_name = create_fname(output_file_name, &outfname_len);
                        write_in_file(output_file_name, str_arr, count);
                        break;
                    case 3:
                        break;
                    default:
                        puts("Please enter 1, 2 or 3");
                        break;
                    }
                } while (!(option == 1 || option == 2 || option == 3));

                puts("Do you want to see result on the screen? Enter");
                puts("1 yes");
                puts("2 no");

                scanf("%d", &option);
                do
                {
                    switch(option)
                    {
                    case 1:
                        puts("Results of processing:");
                        for(i=0; i<count; i++)
                        {
                            printf("%s\n", str_arr[i]);
                            free(str_arr[i]);
                        }
                        free(str_arr);
                        break;
                    case 2:
                        break;
                    default:
                        puts("Please enter 1 or 2");
                        break;
                    }
                } while (!(option == 1 || option == 2));
                free(new_str);
                free(word);
                free(input_file_name);
                free(output_file_name);


                puts("\nProcessing is done");
            }
            else puts("Error at array allocation");
            free(str1);
        }
        else puts("No memory for data!");
        if(fclose(df) == EOF) printf("Closing input file error code: %d\n", errno);
    }
    else perror("Data error");

    printf("Do you want to restart? Enter\n");
    printf("1 to restart\n");
    printf("2 to exit\n");

    scanf("%d", &option);
    do
    {
        switch(option)
        {
        case 1:
            CLS; /**Clearing the console**/
            break;
        case 2:
            exit(0);
            break;
        default:
            puts("Please enter 1 or 2");
            break;
        }
    } while (!(option == 1 || option == 2));
    }
    return 0;
}


char *get_str(int max_len, int *len1)
{
    char *s = NULL;

    s = (char*)malloc(max_len*sizeof(char));
    if(s!=NULL)
    {
        fgets(s, max_len ,stdin);
        *len1 = strlen(s);
        s[*len1-1] = '\0';
        *len1 = strlen(s);
        s = realloc(s, (*len1+1)*sizeof(char));
    }
    else puts("Error at string allocation!");
    return s;
}

int word_in_string(char *word, char *string)
{
    int i, j, flag, len, string_len, w_len, result;
    flag = 0;
    len = 0;
    result = 0;

    string_len = strlen(string);
    w_len = strlen(word);

    for(i = 0; i <= string_len - 1; i++)
    {
        flag = 0;

        if(!isalnum(string[i])) len = 0;
        else len += 1;

        if ((len == w_len) && !isalnum(string[i+1])) {
            for(j = 0; j < w_len; j++) {
                if(tolower(string[i-j]) != tolower(word[w_len-j-1])) flag = 1;
            }
        if (flag == 0) result = 1;
        }
    }
    return result;
}

void write_in_file(char *file_name, char **array_of_strings, int len_of_array)
{
    int i;
    FILE *df;

    df=fopen(file_name, "w");
    if(df != NULL)
    {
        for(i = 0; i < len_of_array; i++)
        {
            fputs(array_of_strings[i], df);
            fputs("\n", df);
        }
        if(fclose(df) != EOF) printf("Check the file\n");
        else printf("Closing output file error code: %d\n", errno);
    }
    else perror("Writing in file data error");
}

int ok_file_name(char *fname, int len)
{
    int i, result,format;

    result = 1;
    if (len < strlen(fname)) len = strlen(fname);

    for (i = 0; i < len; i++) if (!(isalnum(fname[i]) || fname[i] == '_' || fname[i] == '.' || fname[i] == '-')) result = 0;
    format = (fname[len - 4] == '.' && fname[len - 3] == 't' && fname[len - 1] == 't' && fname[len - 2] == 'x'); /**If it
    is equal to 1, then the name ends with ".txt", otherwise it does not.**/
    if (!format) strcat(fname, ".txt");
    return result;
}


char *create_fname(char *fname, int *len)
{
    do
    {
        fname = get_str(MAXLEN - 4, len); /**4 is length of ".txt"**/
        if (!ok_file_name(fname, *len)) printf("Please use only numbers, latin letters and symbols _-.\n");
    } while(!ok_file_name(fname, *len));
    return fname;
}
