#pragma once
#include <vector>

using namespace System;
using namespace System::Collections::Generic;

public ref class MatrixUtils abstract sealed
{
public:
    // Рекурсивное вычисление определителя (работает с std::vector)
    static double Determinant(std::vector<std::vector<double>> mat, int n)
    {
        if (n == 1) return mat[0][0];
        if (n == 2) return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

        double det = 0.0;
        for (int p = 0; p < n; p++)
        {
            // Минор размера n-1
            std::vector<std::vector<double>> minor(n - 1, std::vector<double>(n - 1));
            for (int i = 1; i < n; i++)
            {
                int j_minor = 0;
                for (int j = 0; j < n; j++)
                {
                    if (j == p) continue;
                    minor[i - 1][j_minor] = mat[i][j];
                    j_minor++;
                }
            }
            double sign = (p % 2 == 0) ? 1.0 : -1.0;
            det += sign * mat[0][p] * Determinant(minor, n - 1);
        }
        return det;
    }

    // Замена столбца col матрицы A на вектор B
    static std::vector<std::vector<double>> ReplaceColumn(
        const std::vector<std::vector<double>>& A,
        const std::vector<double>& B, int col, int n)
    {
        std::vector<std::vector<double>> result = A;
        for (int i = 0; i < n; i++)
            result[i][col] = B[i];
        return result;
    }

    // Решение методом Крамера. Возвращает true, если решение найдено.
    static bool SolveCramer(
        const std::vector<std::vector<double>>& A,
        const std::vector<double>& B,
        int n,
        std::vector<double>& X,
        double& mainDet,
        std::vector<double>& auxDets)
    {
        mainDet = Determinant(A, n);
        if (Math::Abs(mainDet) < 1e-12) return false;

        X.resize(n);
        auxDets.resize(n);
        for (int k = 0; k < n; k++)
        {
            auto Ak = ReplaceColumn(A, B, k, n);
            double detK = Determinant(Ak, n);
            auxDets[k] = detK;
            X[k] = detK / mainDet;
        }
        return true;
    }
};