#ifndef DEALEXPRESSION_H
#define DEALEXPRESSION_H


class dealexpression
{
public:
    dealexpression();
    dealexpression(char* expression);
    ~dealexpression();
    void PushStack(char e);
    char PopStack();
    void DealStack(char* expre, int length);
    char* GetResult();
private:
    char* base;
    char* top;
    int stack_size;
    char* result;
};

#endif // DEALEXPRESSION_H
