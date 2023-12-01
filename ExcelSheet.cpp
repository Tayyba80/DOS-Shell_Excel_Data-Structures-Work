#include <iostream>
#include<conio.h>
#include <vector>
#include<Windows.h>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<string>
#include<exception>

using namespace std;

void setColor(int colorCode) {
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(color, colorCode);
}

class Cell {
public:
    string data;
    int row;
    int column;
    Cell* up;
    Cell* down;
    Cell* left;
    Cell* right;
    Cell(int r, int c)
    {
        row = r;
        column = c;
    }

    Cell()
    {
        data = " ";
        up = nullptr;
        down = nullptr;
        left = nullptr;
        right = nullptr;
    }

};

class MiniExcel {
private:
    vector<vector<string>> cut_copy;
    int rows;
    int columns;
    int maxLength;
    Cell* head;
    Cell* current;

public:
    MiniExcel(int rows = 5, int columns = 5, int maxLength = 6) : rows(rows), columns(columns), maxLength(maxLength) {
        head = new Cell();
        current = head;
        initializeGrid();
    }

    void initializeGrid() {
        Cell* temp = head;
        for (int i = 0; i < columns - 1; i++) {
            temp->right = new Cell();
            temp->right->left = temp;
            temp = temp->right;
        }

        Cell* rowStart = head;

        for (int i = 0; i < rows; i++) {
            Cell* prevRow = rowStart;
            Cell* temp = rowStart->down = new Cell();
            temp->up = prevRow;

            for (int j = 0; j < columns - 1; j++) {
                temp->right = new Cell();
                temp->right->left = temp;
                temp->right->up = temp->up->right;
                temp->up->right->down = temp->right;

                temp = temp->right;
                prevRow = prevRow->right;
            }

            if (i > 0) {
                rowStart = rowStart->down;
            }
        }
    }
    void InsertColumnToRight()
    {

        Cell* temp = current;
        Cell* curr = new Cell();

        while (temp != nullptr)
        {
            curr = temp;
            temp = temp->up;
        }
        Cell* curr2 = curr;
        while (curr != nullptr)
        {
            Cell* newCell = new Cell();
            curr->right = newCell;
            curr->right->left = curr;
            curr = curr->down;
        }
        Cell* temp2 = curr2->down;
        curr = curr2;
        while (temp2 != nullptr) {
            temp2->right->up = curr->right;
            curr->right->down = temp2->right;
            curr = curr->down;
            temp2 = temp2->down;
        }

    }
    void InsertColumnToLeft()
    {
        Cell* temp = current;
        Cell* curr = new Cell();

        while (temp != nullptr)
        {
            curr = temp;
            temp = temp->up;
        }
        Cell* curr2 = curr;
        while (curr != nullptr)
        {
            Cell* newCell = new Cell();
            curr->left = newCell;
            curr->left->right = curr;
            curr = curr->down;
        }
        Cell* temp2 = curr2->down;
        curr = curr2;
        while (temp2 != nullptr) {
            temp2->left->up = curr->left;
            curr->left->down = temp2->left;
            curr = curr->down;
            temp2 = temp2->down;
        }
        head = curr2->left;

    }
    void InsertRowAbove()
    {
        Cell* temp = current;

        Cell* curr = new Cell();

        while (temp != nullptr)
        {
            curr = temp;
            temp = temp->left;
        }
        Cell* curr2 = curr;
        while (curr != nullptr)
        {
            Cell* newCell = new Cell();
            curr->up = newCell;
            curr->up->down = curr;
            curr = curr->right;
        }
        Cell* temp2 = curr2->right;
        curr = curr2;
        while (temp2 != nullptr) {
            curr->up->right = temp2->up;
            temp2->up->left = curr->up;
            curr = curr->right;
            temp2 = temp2->right;
        }
        head = curr2->up;
    }
    void InsertRowBelow()
    {
        Cell* temp = current;
        Cell* curr = new Cell();

        while (temp != nullptr)
        {
            curr = temp;
            temp = temp->left;
        }
        Cell* curr2 = curr;
        while (curr != nullptr)
        {
            Cell* newCell = new Cell();
            curr->down = newCell;
            curr->down->up = curr;
            curr = curr->right;
        }
        Cell* temp2 = curr2->right;
        curr = curr2;
        while (temp2 != nullptr) {
            curr->down->right = temp2->down;
            temp2->down->left = curr->down;
            curr = curr->right;
            temp2 = temp2->right;
        }

    }
    void InsertCellByRightShift() {
        Cell* temp = current;
        string currVal = temp->data;

        while (temp->right != nullptr) {
            string rightVal = temp->right->data;
            temp->right->data = currVal;
            currVal = rightVal;
            temp = temp->right;
        }

        Cell* lastColCell = temp;
        Cell* curr = new Cell();

        while (temp != nullptr)
        {
            curr = temp;
            temp = temp->up;
        }
        Cell* curr2 = curr;
        while (curr != nullptr)
        {
            Cell* newCell = new Cell();
            curr->right = newCell;
            curr->right->left = curr;
            curr = curr->down;
        }
        Cell* temp2 = curr2->down;
        curr = curr2;
        while (temp2 != nullptr) {
            temp2->right->up = curr->right;
            curr->right->down = temp2->right;
            curr = curr->down;
            temp2 = temp2->down;
        }
        temp = lastColCell;
        temp->right->data = currVal;
        current = current->right;
        current->left->data = " ";
    }
    void InsertCellByDownShift()
    {
        Cell* temp = current;
        string valueToInsert = temp->data; // Value to insert in the new column

        while (temp->down != nullptr) {
            string tempValue = temp->down->data;
            temp->down->data = valueToInsert;
            valueToInsert = tempValue;
            temp = temp->down;
        }

        Cell* lastRowCell = temp;
        Cell* curr = new Cell();

        while (temp != nullptr)
        {
            curr = temp;
            temp = temp->left;
        }
        Cell* curr2 = curr;
        while (curr != nullptr)
        {
            Cell* newCell = new Cell();
            curr->down = newCell;
            curr->down->up = curr;
            curr = curr->right;
        }
        Cell* temp2 = curr2->right;
        curr = curr2;
        while (temp2 != nullptr) {
            curr->down->right = temp2->down;
            temp2->down->left = curr->down;
            curr = curr->right;
            temp2 = temp2->right;
        }
        temp = lastRowCell;
        temp->down->data = valueToInsert;
        current = current->down;
        current->up->data = " ";
    }
    void DeleteColumn()
    {
        if (current->left != nullptr)
        {
            Cell* temp = current;
            Cell* currLft = current->left;
            Cell* curRight = current->right;

            while (temp->up != nullptr)
            {
                temp = temp->up;
            }

            while (temp != nullptr)
            {
                if (temp->right != nullptr)
                {
                    temp->right->left = temp->left;
                }
                if (temp->left != nullptr)
                {
                    temp->left->right = temp->right;
                }
                Cell* toDel = temp;
                temp = temp->down;
                delete toDel;
            }

            if (currLft != nullptr)
            {
                current = currLft;
            }
            else
            {
                current = curRight;
            }
        }
    }
    void DeleteRow()
    {
        if (current->up != nullptr)
        {
            Cell* temp = current;
            Cell* currUp = current->up;
            Cell* curDown = current->down;

            while (temp->left != nullptr)
            {
                temp = temp->left;
            }

            while (temp != nullptr)
            {
                if (temp->up != nullptr)
                {
                    temp->up->down = temp->down;
                }
                if (temp->down != nullptr)
                {
                    temp->down->up = temp->up;
                }
                Cell* toDel = temp;
                temp = temp->right;
                delete toDel;
            }

            if (currUp != nullptr)
            {
                current = currUp;
            }
            else
            {
                current = curDown;
            }
        }
    }
    void ClearColumn()
    {
        Cell* temp = current;
        while (temp->up != nullptr)
        {
            temp = temp->up;
        }

        while (temp != nullptr)
        {
            temp->data = " ";
            temp = temp->down;
        }
    }
    void ClearRow()
    {
        Cell* temp = current;
        while (temp->left != nullptr)
        {
            temp = temp->left;
        }

        while (temp != nullptr)
        {
            temp->data = " ";
            temp = temp->right;
        }
    }
    void DeleteCellByLeftShift()
    {
        Cell* temp = current;
        while (temp->left != nullptr) {
            string tempValue = temp->left->data;
            temp->data = tempValue;
            temp = temp->left;
        }
        temp->data = " ";
    }
    void DeleteCellByUpShift()
    {
        Cell* temp = current;
        while (temp->up != nullptr) {
            string tempValue = temp->up->data;
            temp->data = tempValue;
            temp = temp->up;
        }
        temp->data = " ";
    }
    void InputRange(Cell*& rangeStart, Cell*& rangeEnd)
    {
        int r, c;
        cout << "Enter range start (row col): ";
        cin >> r >> c;
        rangeStart = new Cell(r, c);
        cout << "Enter range end (row col): ";
        cin >> r >> c;
        rangeEnd = new Cell(r, c);
    }
    void InputRangeWithCell(Cell*& rangeStart, Cell*& rangeEnd, Cell*& result)
    {
        int r, c;
        cout << "Enter range start (row col): ";
        cin >> r >> c;
        rangeStart = new Cell(r, c);
        cout << "Enter range end (row col): ";
        cin >> r >> c;
        rangeEnd = new Cell(r, c);
        cout << "Enter cells Coordinates for result display (row col): ";
        cin >> r >> c;
        result = new Cell(r, c);
    }
    void InputRangeForPaste(Cell*& rangeStart)
    {
        int r, c;
        cout << "Enter range start (row col): ";
        cin >> r >> c;
        rangeStart = new Cell(r, c);
    }
    string getData(int row, int col)
    {
        Cell* temp = head; int r = 0;
        while (temp != nullptr && r != row)
        {
            temp = temp->down;
            r++;
        }
        if (temp == nullptr)
        {
            return "";
        }
        int c = 0;
        while (temp != nullptr && c != col)
        {
            temp = temp->right;
            c++;
        }
        if (temp == nullptr)
        {
            return "";
        }
        return temp->data;
    }
    Cell* getCellForResult(Cell* result)
    {
        Cell* temp = head; int r = 0;
        while (temp != nullptr && r != result->row)
        {
            temp = temp->down;
            r++;
        }
        if (temp == nullptr)
        {
            return nullptr;
        }
        int c = 0;
        while (temp != nullptr && c != result->column)
        {
            temp = temp->right;
            c++;
        }
        if (temp == nullptr)
        {
            return nullptr;
        }
        return temp;
    }
    int GetRangeSum(Cell*& rangeStart, Cell*& rangeEnd)
    {
        int sum = 0;
        try
        {
            for (int row = rangeStart->row; row <= rangeEnd->row; row++)
            {
                for (int col = rangeStart->column; col <= rangeEnd->column; col++)
                {
                    string val = getData(row, col);
                    if (val != " ")
                    {
                        sum += stoi(val);
                    }
                }
            }
            return sum;
        }
        catch (const invalid_argument& e)
        {
            cout << "Invalid Data" << e.what() << endl;
        }
    }
    int GetRangeAverage(Cell*& rangeStart, Cell*& rangeEnd)
    {
        int count = 0;
        int sum = 0;
        try
        {
            for (int row = rangeStart->row; row <= rangeEnd->row; row++)
            {
                for (int col = rangeStart->column; col <= rangeEnd->column; col++)
                {
                    string val = getData(row, col);
                    if (val != " ")
                    {
                        sum += stoi(val);
                        count++;
                    }

                }
            }
            int avg = sum / count;
            return avg;
        }
        catch (const invalid_argument& e)
        {
            cout << "Invalid Data" << e.what() << endl;
        }
    }
    int GetRangeCount(Cell*& rangeStart, Cell*& rangeEnd)
    {
        int count = 0;
        try {
            for (int row = rangeStart->row; row <= rangeEnd->row; row++)
            {
                for (int col = rangeStart->column; col <= rangeEnd->column; col++)
                {
                    count++;
                }
            }

            return count;
        }
        catch (const invalid_argument& e)
        {
            cout << "Invalid Data" << e.what() << endl;
        }
    }
    int GetRangeMin(Cell*& rangeStart, Cell*& rangeEnd)
    {
        try {
            Cell* temp = getCellForResult(rangeStart);
            int min = stoi(temp->data);
            for (int row = rangeStart->row; row <= rangeEnd->row; row++)
            {
                for (int col = rangeStart->column; col <= rangeEnd->column; col++)
                {
                    string val = getData(row, col);
                    if (val != " " && val < to_string(min))
                        min = stoi(val);
                }
            }
            return min;
        }
        catch (const invalid_argument& e)
        {
            cout << "Invalid Data" << e.what() << endl;
        }
    }
    int GetRangeMax(Cell*& rangeStart, Cell*& rangeEnd)
    {
        try {
            Cell* temp = getCellForResult(rangeStart);
            int max = stoi(temp->data);

            for (int row = rangeStart->row; row <= rangeEnd->row; row++)
            {
                for (int col = rangeStart->column; col <= rangeEnd->column; col++)
                {
                    string val = getData(row, col);
                    if (val != " " && val > to_string(max))
                        max = stoi(val);
                }
            }
            return max;
        }
        catch (const invalid_argument& e)
        {
            cout << "Invalid Data" << e.what() << endl;
        }
    }
    void cut(Cell*& rangeStart, Cell*& rangeEnd)
    {
        try
        {
            int i = 0;
            cut_copy.clear();
            for (int row = rangeStart->row; row <= rangeEnd->row; row++)
            {
                vector<string> rowVal;
                for (int col = rangeStart->column; col <= rangeEnd->column; col++)
                {
                    Cell* temp = new Cell(row, col);
                    Cell* wantedCell = getCellForResult(temp);
                    string val = wantedCell->data;
                    rowVal.push_back(val);
                    wantedCell->data = " ";
                }
                cut_copy.push_back(rowVal);
            }
        }
        catch (const invalid_argument& e)
        {
            cout << "Invalid Data" << e.what() << endl;
        }
    }
    void copy(Cell*& rangeStart, Cell*& rangeEnd)
    {
        try {
            cut_copy.clear();
            for (int row = rangeStart->row; row <= rangeEnd->row; row++)
            {
                vector<string> rowVal;
                for (int col = rangeStart->column; col <= rangeEnd->column; col++)
                {
                    string val = getData(row, col);
                    rowVal.push_back(val);
                }
                cut_copy.push_back(rowVal);
            }
        }
        catch (const invalid_argument& e)
        {
            cout << "Invalid Data" << e.what() << endl;
        }
    }
    void paste(Cell* RangeStart)
    {
        int totalRows = cut_copy.size();
        int totalCols = cut_copy[0].size();

        int startRow = RangeStart->row;
        int startCol = RangeStart->column;

        for (int i = 0; i < totalRows && startRow <= rows; ++i, ++startRow)
        {
            int col = startCol;
            for (int j = 0; j < totalCols && col <= columns; ++j, ++col)
            {

                Cell* cell = getCellForResult(new Cell(startRow, col));

                if (cell)
                    cell->data = cut_copy[i][j];


            }
        }
    }
    void printGrid() {
        Cell* temp = head;

        while (temp != nullptr) {
            Cell* temp2 = temp;

            while (temp2 != nullptr) {
                cout << "+";
                for (int i = 0; i < maxLength; i++)
                {
                    cout << "-";
                }
                temp2 = temp2->right;
            }
            cout << "+";
            cout << endl;

            temp2 = temp;
            while (temp2 != nullptr) {

                if (temp2 == current) {
                    setColor(9);
                    cout << "|";
                    setColor(9);
                    string val = temp2->data.substr(0, maxLength);
                    cout << setw(maxLength) << left << val;

                    setColor(7);
                }
                else {
                    cout << "|";
                    string val = temp2->data.substr(0, maxLength);
                    cout << setw(maxLength) << left << val;
                }
                temp2 = temp2->right;
            }
            cout << "|" << endl;

            temp = temp->down;
        }

        Cell* rowStart = head;
        while (rowStart != nullptr) {
            cout << "+";
            for (int i = 0; i < maxLength; i++)
            {
                cout << "-";
            }
            rowStart = rowStart->right;
        }
        cout << "+" << endl;
    }
    void writeDataInFile(string name)
    {
        ofstream writer(name);
        Cell* temp = head;

        while (temp != nullptr)
        {
            Cell* temp2 = temp;

            while (temp2 != nullptr)
            {
                writer << temp2->data << ",";
                temp2 = temp2->right;
            }
            writer << endl;
            temp = temp->down;
        }

    }
    void readDataFromFile(string name)
    {
        ifstream reader(name);
        if (!reader.is_open()) {
            cout << "File " << name << " does not exist" << endl;
            return;
        }

        string line;
        Cell* temp = head;
        Cell* temp2 = head;

        while (getline(reader, line)) {
            istringstream ss(line);
            string cellData;

            while (getline(ss, cellData, ','))
            {
                if (temp != nullptr)
                {
                    temp->data = cellData;

                    if (temp->right != nullptr)
                    {
                        temp = temp->right;
                    }
                    else
                    {
                        current = temp;
                        InsertColumnToRight();
                        temp = temp->right;
                    }
                }
            }
            if (temp2->down != nullptr)
            {
                temp2 = temp2->down;
                temp = temp2;
            }
            else
            {
                current = temp2;
                InsertRowBelow();
                temp2 = temp2->down;
                temp = temp2;
            }
        }

        reader.close();
    }
    void moveCurrentCellWithInput()
    {
        vector<vector<string>>cut_copy = {};
        while (true) {
            if (_kbhit())
            {
                int key = _getch();
                if (key == 224)
                {
                    key = _getch();
                    switch (key)
                    {
                    case 75:
                        if (current->left != nullptr)
                        {
                            current = current->left;
                            system("cls");
                            printGrid();
                            Sleep(100);
                        }
                        else
                        {
                            InsertColumnToLeft();
                            system("cls");
                            printGrid();
                            Sleep(100);
                        }

                        break;
                    case 77:
                        if (current->right != nullptr) {
                            current = current->right;
                            system("cls");
                            printGrid();
                            Sleep(100);
                        }
                        else
                        {
                            InsertColumnToRight();
                            system("cls");
                            printGrid();
                            Sleep(100);
                        }

                        break;
                    case 72:
                        if (current->up != nullptr) {
                            current = current->up;
                            system("cls");
                            printGrid();
                            Sleep(100);
                        }
                        else
                        {
                            InsertRowAbove();
                            system("cls");
                            printGrid();
                            Sleep(100);
                        }
                        break;

                    case 80:
                        if (current->down != nullptr) {
                            current = current->down;
                            system("cls");
                            printGrid();
                            Sleep(100);
                        }
                        else
                        {
                            InsertRowBelow();
                            system("cls");
                            printGrid();
                            Sleep(100);
                        }
                        break;
                    }
                }

                else if (key == 82)//R
                {
                    InsertCellByRightShift();
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 68)//D
                {
                    InsertCellByDownShift();
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 67)//C
                {
                    DeleteColumn();
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 76)//L
                {
                    DeleteRow();
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 78)//N
                {
                    ClearColumn();
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 83)//S
                {
                    ClearRow();
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 87)//W
                {
                    DeleteCellByLeftShift();
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 81)//Q
                {
                    DeleteCellByUpShift();
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 90)//Z
                {
                    Cell* rangeStart;
                    Cell* rangeEnd;
                    InputRange(rangeStart, rangeEnd);
                    int sum = GetRangeSum(rangeStart, rangeEnd);
                    cout << sum;
                    Sleep(10000);
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 84)//T
                {
                    Cell* rangeStart;
                    Cell* rangeEnd;
                    InputRange(rangeStart, rangeEnd);
                    int avg = GetRangeAverage(rangeStart, rangeEnd);
                    cout << avg;
                    Sleep(1000);
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 85)//U
                {
                    Cell* rangeStart;
                    Cell* rangeEnd;
                    Cell* result;
                    InputRangeWithCell(rangeStart, rangeEnd, result);
                    int avg = GetRangeAverage(rangeStart, rangeEnd);
                    string average = to_string(avg);
                    Cell* res = getCellForResult(result);
                    res->data = average;
                    Sleep(1000);
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 79)//O
                {
                    Cell* rangeStart;
                    Cell* rangeEnd;
                    Cell* result;
                    InputRangeWithCell(rangeStart, rangeEnd, result);
                    int sum = GetRangeSum(rangeStart, rangeEnd);
                    string Add = to_string(sum);
                    Cell* res = getCellForResult(result);
                    res->data = Add;
                    Sleep(1000);
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 71)//G
                {
                    Cell* rangeStart;
                    Cell* rangeEnd;
                    Cell* result;
                    InputRangeWithCell(rangeStart, rangeEnd, result);
                    int count = GetRangeCount(rangeStart, rangeEnd);
                    string num = to_string(count);
                    Cell* res = getCellForResult(result);
                    res->data = num;
                    Sleep(1000);
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 73)//I
                {
                    Cell* rangeStart;
                    Cell* rangeEnd;
                    Cell* result;
                    InputRangeWithCell(rangeStart, rangeEnd, result);
                    int min = GetRangeMin(rangeStart, rangeEnd);
                    string mini = to_string(min);
                    Cell* res = getCellForResult(result);
                    res->data = mini;
                    Sleep(1000);
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 74)//J
                {
                    Cell* rangeStart;
                    Cell* rangeEnd;
                    Cell* result;
                    InputRangeWithCell(rangeStart, rangeEnd, result);
                    int max = GetRangeMax(rangeStart, rangeEnd);
                    string maximum = to_string(max);
                    Cell* res = getCellForResult(result);
                    res->data = maximum;
                    Sleep(1000);
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 86)//V
                {
                    Cell* rangeStart;
                    InputRangeForPaste(rangeStart);
                    paste(rangeStart);
                    Sleep(1000);
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 88)//X
                {
                    Cell* rangeStart;
                    Cell* rangeEnd;
                    InputRange(rangeStart, rangeEnd);
                    cut(rangeStart, rangeEnd);
                    Sleep(1000);
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 72)//H
                {
                    Cell* rangeStart;
                    Cell* rangeEnd;
                    InputRange(rangeStart, rangeEnd);
                    copy(rangeStart, rangeEnd);
                    Sleep(1000);
                    system("cls");
                    printGrid();
                    Sleep(100);
                }
                else if (key == 80)//P
                {
                    writeDataInFile("ExcelSheet.txt");
                    /* printGrid();
                     Sleep(100);*/
                }
                else if (key == 65)//A
                {
                    string val;
                    cout << "Enter Value";
                    cout << endl;
                    getline(cin, val);
                    current->data = val;
                    system("cls");
                    printGrid();
                    Sleep(100);
                }

            }
        }
    }
};
void instructions() 
{
    cout << "Instructions:\n";
    cout << "Arrow Keys (Up, Down, Left, Right): Move current cell\n";
    cout << "R: Insert Cell by Right Shift\n";
    cout << "D: Insert Cell by Down Shift\n";
    cout << "C: Delete Column\n";
    cout << "L: Delete Row\n";
    cout << "N: Clear Column\n";
    cout << "S: Clear Row\n";
    cout << "W: Delete Cell by Left Shift\n";
    cout << "Q: Delete Cell by Up Shift\n";
    cout << "Z: Get Range Sum\n";
    cout << "T: Get Range Average\n";
    cout << "U: Get Range Average and Update Cell\n";
    cout << "O: Get Range Sum and Update Cell\n";
    cout << "G: Get Range Count and Update Cell\n";
    cout << "I: Get Range Min and Update Cell\n";
    cout << "J: Get Range Max and Update Cell\n";
    cout << "V: Paste\n";
    cout << "X: Cut\n";
    cout << "H: Copy\n";
    cout << "P: Write Data to File\n";
    cout << "A: Enter Value into Current Cell\n";
    _getch();
}

void mainMenu() {
    system("cls");
    cout << "MiniExcel Menu" << endl;
    cout << "1. See Instructions" << endl;
    cout << "2. Start Excel" << endl;
    cout << "3. Quit" << endl;
}

int main()
{
    MiniExcel excel;
    while (true) 
    {
        mainMenu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            instructions();
            break;
        case 2:
            system("cls");
            excel.readDataFromFile("ExcelSheet.txt");
            excel.printGrid();
            excel. moveCurrentCellWithInput();
            break;
        case 3:
            cout << "Thanks for using MiniExcel. Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice. Please select a valid option. Press any key to try again!" << endl;
            _getch();
        }
    }

    
}