#pragma once
#include "MatrixUtils.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

namespace CramerSolver {
    using namespace System::Diagnostics;
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class Form1 : public Form
    {
    private:
        NumericUpDown^ nudDimension;
        DataGridView^ dgvMatrix;
        DataGridView^ dgvConstants;
        Button^ btnSolve;
        Button^ btnClear;
        Button^ btnExample;
        Button^ btnRandom;
        TextBox^ txtLog;
        Label^ lblStatus;
        GroupBox^ groupMatrix;
        GroupBox^ groupConstants;
        GroupBox^ groupResults;

    public:
        Form1()
        {
            InitializeComponent();
            // Установка начальной размерности 3
            nudDimension->Value = 3;
            UpdateGrids();
        }

    protected:
        ~Form1()
        {
            if (components)
                delete components;
        }

    private:
        System::ComponentModel::Container^ components;
        void InitializeComponent()
        {
            this->nudDimension = gcnew NumericUpDown();
            this->dgvMatrix = gcnew DataGridView();
            this->dgvConstants = gcnew DataGridView();
            this->btnSolve = gcnew Button();
            this->btnClear = gcnew Button();
            this->btnExample = gcnew Button();
            this->txtLog = gcnew TextBox();
            this->lblStatus = gcnew Label();
            this->groupMatrix = gcnew GroupBox();
            this->groupConstants = gcnew GroupBox();
            this->groupResults = gcnew GroupBox();

            // Настройка формы
            this->Text = L"CramerSolver - Решение СЛАУ методом Крамера";
            this->Size = System::Drawing::Size(750, 600);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;

            // NumericUpDown
            this->nudDimension->Location = Point(12, 12);
            this->nudDimension->Size = System::Drawing::Size(60, 22);
            this->nudDimension->Minimum = 2;
            this->nudDimension->Maximum = 10;
            this->nudDimension->Value = 3;
            this->nudDimension->ValueChanged += gcnew EventHandler(this, &Form1::nudDimension_ValueChanged);

            Label^ lblDim = gcnew Label();
            lblDim->Text = L"Размерность (n):";
            lblDim->Location = Point(12, 12);
            lblDim->AutoSize = true;
            // скорректируем позицию NumericUpDown
            this->nudDimension->Location = Point(lblDim->Right + 6, 10);

            // GroupBox матрицы
            this->groupMatrix->Text = L"Матрица коэффициентов A";
            this->groupMatrix->Location = Point(12, 40);
            this->groupMatrix->Size = System::Drawing::Size(350, 250);
            this->groupMatrix->Controls->Add(this->dgvMatrix);
            this->dgvMatrix->Location = Point(6, 20);
            this->dgvMatrix->Size = System::Drawing::Size(338, 220);
            this->dgvMatrix->AllowUserToAddRows = false;
            this->dgvMatrix->AllowUserToDeleteRows = false;
            this->dgvMatrix->AllowUserToResizeColumns = true;
            this->dgvMatrix->AllowUserToResizeRows = false;
            this->dgvMatrix->ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dgvMatrix->CellValidating += gcnew DataGridViewCellValidatingEventHandler(this, &Form1::dgv_CellValidating);
            this->dgvMatrix->CellEndEdit += gcnew DataGridViewCellEventHandler(this, &Form1::dgv_CellEndEdit);

            // GroupBox вектора
            this->groupConstants->Text = L"Вектор свободных членов B";
            this->groupConstants->Location = Point(370, 40);
            this->groupConstants->Size = System::Drawing::Size(160, 250);
            this->groupConstants->Controls->Add(this->dgvConstants);
            this->dgvConstants->Location = Point(6, 20);
            this->dgvConstants->Size = System::Drawing::Size(148, 220);
            this->dgvConstants->AllowUserToAddRows = false;
            this->dgvConstants->AllowUserToDeleteRows = false;
            this->dgvConstants->AllowUserToResizeColumns = false;
            this->dgvConstants->AllowUserToResizeRows = false;
            this->dgvConstants->ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dgvConstants->CellValidating += gcnew DataGridViewCellValidatingEventHandler(this, &Form1::dgv_CellValidating);
            this->dgvConstants->CellEndEdit += gcnew DataGridViewCellEventHandler(this, &Form1::dgv_CellEndEdit);

            // Кнопки
            this->btnSolve->Text = L"Решить";
            this->btnSolve->Location = Point(12, 300);
            this->btnSolve->Size = System::Drawing::Size(100, 30);
            this->btnSolve->Click += gcnew EventHandler(this, &Form1::btnSolve_Click);

            this->btnClear->Text = L"Очистить";
            this->btnClear->Location = Point(120, 300);
            this->btnClear->Size = System::Drawing::Size(100, 30);
            this->btnClear->Click += gcnew EventHandler(this, &Form1::btnClear_Click);

            this->btnExample->Text = L"Пример 3x3";
            this->btnExample->Location = Point(228, 300);
            this->btnExample->Size = System::Drawing::Size(100, 30);
            this->btnExample->Click += gcnew EventHandler(this, &Form1::btnExample_Click);

            this->btnRandom = gcnew Button();
            this->btnRandom->Text = L"Случайно";
            this->btnRandom->Location = Point(336, 300);
            this->btnRandom->Size = System::Drawing::Size(100, 30);
            this->btnRandom->Click += gcnew EventHandler(this, &Form1::btnRandom_Click);
            this->Controls->Add(this->btnRandom);

            // GroupBox результатов
            this->groupResults->Text = L"Результаты вычислений";
            this->groupResults->Location = Point(12, 340);
            this->groupResults->Size = System::Drawing::Size(710, 190);
            this->groupResults->Controls->Add(this->txtLog);
            this->txtLog->Location = Point(6, 20);
            this->txtLog->Size = System::Drawing::Size(698, 135);
            this->txtLog->Multiline = true;
            this->txtLog->ReadOnly = true;
            this->txtLog->ScrollBars = ScrollBars::Vertical;
            this->txtLog->Font = gcnew System::Drawing::Font(L"Consolas", 9);

            // Статус
            this->lblStatus->Text = L"Готов";
            this->lblStatus->Location = Point(12, 535);
            this->lblStatus->AutoSize = true;

            // Добавление на форму
            this->Controls->Add(lblDim);
            this->Controls->Add(this->nudDimension);
            this->Controls->Add(this->groupMatrix);
            this->Controls->Add(this->groupConstants);
            this->Controls->Add(this->btnSolve);
            this->Controls->Add(this->btnClear);
            this->Controls->Add(this->btnExample);
            this->Controls->Add(this->groupResults);
            this->Controls->Add(this->lblStatus);
        }

        // Обновление размеров таблиц при смене размерности
        void UpdateGrids()
        {
            int n = (int)nudDimension->Value;

            // Матрица коэффициентов
            dgvMatrix->Columns->Clear();
            for (int i = 0; i < n; i++)
            {
                String^ name = "x" + (i + 1).ToString();
                dgvMatrix->Columns->Add(name, name);
                dgvMatrix->Columns[i]->Width = 60;
            }
            dgvMatrix->Rows->Clear();
            dgvMatrix->Rows->Add(n);
            for (int i = 0; i < n; i++)
                dgvMatrix->Rows[i]->HeaderCell->Value = (i + 1).ToString();

            // Вектор свободных членов
            dgvConstants->Columns->Clear();
            dgvConstants->Columns->Add("B", "B");
            dgvConstants->Columns[0]->Width = 80;
            dgvConstants->Rows->Clear();
            dgvConstants->Rows->Add(n);
            for (int i = 0; i < n; i++)
                dgvConstants->Rows[i]->HeaderCell->Value = (i + 1).ToString();
        }

        // Обработчик изменения размерности
        System::Void nudDimension_ValueChanged(System::Object^ sender, System::EventArgs^ e)
        {
            UpdateGrids();
        }

        // Проверка ввода (только числа)
        System::Void dgv_CellValidating(System::Object^ sender, DataGridViewCellValidatingEventArgs^ e)
        {
            DataGridView^ dgv = safe_cast<DataGridView^>(sender);
            String^ value = e->FormattedValue->ToString();

            // Пустые ячейки разрешаем без проверки
            if (String::IsNullOrWhiteSpace(value))
                return;

            double tmp;
            if (!Double::TryParse(value, tmp))
            {
                // Показываем предупреждение
                MessageBox::Show(L"Допустимы только числовые значения.\nПожалуйста, введите число.",
                    L"Ошибка ввода",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Warning);

                dgv->Rows[e->RowIndex]->Cells[e->ColumnIndex]->ErrorText = L"Введите число";
                e->Cancel = true;
            }
        }

        // Сброс ошибки после редактирования
        System::Void dgv_CellEndEdit(System::Object^ sender, DataGridViewCellEventArgs^ e)
        {
            DataGridView^ dgv = safe_cast<DataGridView^>(sender);
            dgv->Rows[e->RowIndex]->Cells[e->ColumnIndex]->ErrorText = String::Empty;
        }

        // Чтение матрицы из DataGridView
        std::vector<std::vector<double>> ReadMatrix(DataGridView^ dgv, int n)
        {
            std::vector<std::vector<double>> mat(n, std::vector<double>(n));
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    Object^ val = dgv->Rows[i]->Cells[j]->Value;
                    if (val == nullptr || String::IsNullOrWhiteSpace(val->ToString()))
                        throw gcnew Exception("Не все ячейки матрицы заполнены.");
                    mat[i][j] = Convert::ToDouble(val);
                }
            }
            return mat;
        }

        // Чтение вектора B из DataGridView
        std::vector<double> ReadConstants(DataGridView^ dgv, int n)
        {
            std::vector<double> vec(n);
            for (int i = 0; i < n; i++)
            {
                Object^ val = dgv->Rows[i]->Cells[0]->Value;
                if (val == nullptr || String::IsNullOrWhiteSpace(val->ToString()))
                    throw gcnew Exception("Не все ячейки вектора B заполнены.");
                vec[i] = Convert::ToDouble(val);
            }
            return vec;
        }

        // Решение
        System::Void btnSolve_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try
            {
                int n = (int)nudDimension->Value;
                auto A = ReadMatrix(dgvMatrix, n);
                auto B = ReadConstants(dgvConstants, n);
                std::vector<double> X;
                double mainDet = 0;
                std::vector<double> auxDets;

                Stopwatch^ sw = gcnew Stopwatch();
                sw->Start();

                bool hasSolution = MatrixUtils::SolveCramer(A, B, n, X, mainDet, auxDets);

                sw->Stop();
                long long elapsedMs = sw->ElapsedMilliseconds;

                if (!hasSolution)
                {
                    txtLog->Text = L"Главный определитель равен нулю. Система не имеет единственного решения.";
                    lblStatus->Text = L"Ошибка: нулевой определитель";
                    return;
                }

                std::wstringstream ss;
                ss << std::fixed << std::setprecision(4);
                ss << L"Главный определитель Δ = " << mainDet << L"\r\n\r\n";
                ss << L"Промежуточные определители:\r\n";
                for (int i = 0; i < n; i++)
                    ss << L"Δ" << (i + 1) << L" = " << auxDets[i] << L"\r\n";
                ss << L"\r\nРешение системы:\r\n";
                for (int i = 0; i < n; i++)
                    ss << L"x" << (i + 1) << L" = " << X[i] << L"\r\n";

                ss << L"\r\nВремя вычисления: " << elapsedMs << L" мс";

                txtLog->Text = gcnew String(ss.str().c_str());
                lblStatus->Text = L"Решение выполнено за " + elapsedMs + L" мс";
            }
            catch (Exception^ ex)
            {
                MessageBox::Show(ex->Message, L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                lblStatus->Text = L"Ошибка при чтении данных";
            }
        }

        // Очистка
        System::Void btnClear_Click(System::Object^ sender, System::EventArgs^ e)
        {
            for each(DataGridViewRow ^ row in dgvMatrix->Rows)
                for each(DataGridViewCell ^ cell in row->Cells)
                    cell->Value = nullptr;
            for each(DataGridViewRow ^ row in dgvConstants->Rows)
                row->Cells[0]->Value = nullptr;
            txtLog->Clear();
            lblStatus->Text = L"Готов";
        }

        // Загрузка тестового примера 3x3 
        System::Void btnExample_Click(System::Object^ sender, System::EventArgs^ e)
        {
            nudDimension->Value = 3; // автоматически вызовет UpdateGrids()

            // Заполнение матрицы A
            double a[3][3] = { {2, 1, -1},
                               {3, -2, 4},
                               {1, 1, 1} };
            double b[3] = { 2, 11, 8 };

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                    dgvMatrix->Rows[i]->Cells[j]->Value = a[i][j];
                dgvConstants->Rows[i]->Cells[0]->Value = b[i];
            }
            lblStatus->Text = L"Загружен пример 3x3";
        }
        /* Для заполнения целыми числами следующая функция
        void RandomFill() {
    Random^ rand = gcnew Random();
    int n = (int)nudDimension->Value;
    int minVal = -100;
    int maxVal = 100;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dgvMatrix->Rows[i]->Cells[j]->Value =
                rand->Next(minVal, maxVal + 1).ToString();

    for (int i = 0; i < n; i++)
        dgvConstants->Rows[i]->Cells[0]->Value =
            rand->Next(minVal, maxVal + 1).ToString();
            }
            */

            // Заполнение случайными числами
        void RandomFill()
        {
            Random^ rand = gcnew Random();
            int n = (int)nudDimension->Value;

            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    dgvMatrix->Rows[i]->Cells[j]->Value =
                    (rand->NextDouble() * 20.0 - 10.0).ToString("F2");

            for (int i = 0; i < n; i++)
                dgvConstants->Rows[i]->Cells[0]->Value =
                (rand->NextDouble() * 20.0 - 10.0).ToString("F2");
        }

        System::Void btnRandom_Click(System::Object^ sender, System::EventArgs^ e)
        {
            RandomFill();
            lblStatus->Text = L"Матрица заполнена случайными числами";
        }
    };
}
