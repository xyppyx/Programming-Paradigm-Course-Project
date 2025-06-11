#include<iostream>
#include<array>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;

class FloatMatrix
{
    private://members
        float m_float_matrix[4][4];

    public://constructors
        FloatMatrix();
        FloatMatrix(const float* array);
        FloatMatrix(const std::array<float, 16> &array);
        FloatMatrix(const FloatMatrix &other_matrix);
        
    public://external interfaces
        FloatMatrix add(const FloatMatrix &other_matrix);
        FloatMatrix multiply(const FloatMatrix &other_matrix);
        void print();
        FloatMatrix& operator+=(const FloatMatrix &other_matrix);
        float& operator()(int row, int col);
        float operator()(int row, int col) const;
        FloatMatrix& operator=(const FloatMatrix &other_matrix);
};

//default constructor
FloatMatrix::FloatMatrix()
{
    cout<<"Please input a 4x4 float matrix:"<<endl;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            cin>>m_float_matrix[i][j];
}

//constructor with a float array
FloatMatrix::FloatMatrix(const float* array)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            m_float_matrix[i][j] = array[i * 4 + j];
}

//constructor with a std::array
FloatMatrix::FloatMatrix(const std::array<float, 16> &array)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            m_float_matrix[i][j] = array[i * 4 + j];
}

//copy constructor
FloatMatrix::FloatMatrix(const FloatMatrix &other_matrix)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            m_float_matrix[i][j] = other_matrix.m_float_matrix[i][j];
}

//add two matrices
FloatMatrix FloatMatrix::add(const FloatMatrix &other_matrix)
{
    FloatMatrix result(*this);
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            result.m_float_matrix[i][j] += other_matrix.m_float_matrix[i][j];
    return result;
}

//multiply two matrices
FloatMatrix FloatMatrix::multiply(const FloatMatrix &other_matrix)
{
    float temp[16] = {0};
    FloatMatrix result(temp);
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            for(int k = 0; k < 4; k++)
                result.m_float_matrix[i][j] += m_float_matrix[i][k] * other_matrix.m_float_matrix[k][j];
    return result;
}

//print the matrix
void FloatMatrix::print()
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
            cout<<m_float_matrix[i][j]<<' ';
        cout<<endl;
    }
}

//operator +=
FloatMatrix& FloatMatrix::operator+=(const FloatMatrix &other_matrix)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            m_float_matrix[i][j] += other_matrix.m_float_matrix[i][j];
    return *this;
}

//operator ()
float& FloatMatrix::operator()(int row, int col)
{
    if(0 <= row && row < 4 && 0 <= col && col < 4)
        return m_float_matrix[row][col];
}

//operator () const
float FloatMatrix::operator()(int row, int col) const
{
    if(0 <= row && row < 4 && 0 <= col && col < 4)
        return m_float_matrix[row][col];
}

//operator =
FloatMatrix& FloatMatrix::operator=(const FloatMatrix &other_matrix)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            m_float_matrix[i][j] = other_matrix.m_float_matrix[i][j];
    return *this;
}

signed main()
{
    std::array<float, 16> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    std::array<float, 16> b = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    FloatMatrix matrix1(a);
    FloatMatrix matrix2(b);
    matrix1.print();
    matrix1 += matrix2;
    matrix1.print();
    matrix1 = matrix1.multiply(matrix2);
    matrix1.print();
    return 0;
}