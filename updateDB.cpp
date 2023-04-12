#include "utility.cpp"

int main ()
{
    DB DB;//create DB and load it
    LoadDB(DB);
    rerunScript(DB);//rerun script
    return 0;
}