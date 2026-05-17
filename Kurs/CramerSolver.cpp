#include "Form1.h"

using namespace System;
using namespace System::Windows::Forms;


int main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew CramerSolver::Form1());
    return 0;
}