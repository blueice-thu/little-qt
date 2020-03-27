#include "dealexpression.h"
#include <string.h>

#define MaxSize 100
#define OverFlow 0

char Symbol2Number(char temp);
char Number2Symbol(char temp);

dealexpression::dealexpression()
{
    base = nullptr;
    top = nullptr;
    stack_size = -1;
    result = nullptr;
}
dealexpression::dealexpression(char* expre)
{
    char emptyResult[6] = "Empty";

    int length = 0;
    while (expre[length] != '\0')
    {
        length++;
    }
    if (length == 0)
    {
        result = new char[6];
        strcpy(result, emptyResult);
        return ;
    }
    base = new char[length + 1];
    top = base;
    stack_size = length + 1;
    result = new char[length + 1];
    DealStack(expre, length);
}
dealexpression::~dealexpression()
{
    if (base != nullptr)
        delete[]base;
    if (result != nullptr)
        delete[]result;
}
void dealexpression::PushStack(char e)
{
    if (top - base >= stack_size)
    {
        return ;
    }
    *(top) = e;
    top++;
}
char Symbol2Number(char temp)
{
    if (temp == '!')
        temp = '1';
    else if (temp == '&')
        temp = '2';
    else if (temp == '|')
        temp = '3';
    else if (temp == '^')
        temp = '4';
    else if (temp == '~')
        temp = '5';
    return temp;
}
char Number2Symbol(char temp)
{
    if (temp == '1')
        temp = '!';
    else if (temp == '2')
        temp = '&';
    else if (temp == '3')
        temp = '|';
    else if (temp == '4')
        temp = '^';
    else if (temp == '5')
        temp = '~';
    return temp;
}
char dealexpression::PopStack()
{
    char temp;
    if (top == base)
        return 0;
    (top)--;
    temp = *(top);
    temp = Number2Symbol(temp);
    return temp;

}
bool checkright(char* e)
{
    int i = 0;
    char symbol[6] = {'!', '&', '|', '^', '~'};
    while (e[i] != '\0')
    {
        if (e[i] >= 'A' && e[i] <= 'Z')
        {
            i++;
            continue;
        }
        else
        {
            bool continue_flag = false;
            for (int j = 0; j < 5; j++)
            {
                if (e[i] == symbol[j])
                {
                    i++;
                    continue_flag = true;
                    break;
                }
            }
            if (continue_flag)
                continue;
        }
        return false;
    }
    return true;
}
void dealexpression::DealStack(char* expre, int length)
{
    char badResult[12] = "Wrong Input";
    if (!checkright(expre))
    {
        if (result != nullptr)
            delete[]result;
        result = badResult;
        return ;
    }
    int k_result = 0;

    char temp;
    int num_letter = 0, num_symbol = 0;
    for (int i = 0; i < length; i++)
    {
        temp = expre[i];
        temp = Symbol2Number(temp);
        if (temp >= 'A' && temp <= 'Z')
        {
            num_letter++;
            result[k_result] = temp;
            k_result++;
            continue;
        }
        else if (temp >= '1' && temp <= '5')
        {
            if (temp != '1')
                num_symbol++;
            if (base == top)
            {
                PushStack(temp);
            }
            else if (*(top - 1) <= temp)
            {
                while (*(top - 1) <= temp && base != top)
                {
                    if (temp != '1')
                    {
                        result[k_result] = PopStack();
                        k_result++;
                    }
                    else
                        break;
                }
                PushStack(temp);
            }
            else if (*(top - 1) > temp)
            {
                PushStack(temp);
            }
        }
    }
    while (base != top)
    {
        result[k_result] = PopStack();
        k_result++;
    }
    result[k_result] = '\0';
    if (num_letter - 1 != num_symbol)
    {
        delete[] result;
        result = badResult;
    }
}
char* dealexpression::GetResult()
{
    return result;
}
