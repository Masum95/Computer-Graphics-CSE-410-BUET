#include <bits/stdc++.h>
using namespace std;

#define PI acos(-1.0)
#define DIM 4
ofstream stage1_out;
ofstream stage2_out;
ofstream stage3_out;

class Matrix
{
    int row, col;
    double **ara;

public:
    Matrix(int r, int c)
    {
        row = r;
        col = c;
        ara = new double *[row];
        for (int i = 0; i < row; i++)
        {
            ara[i] = new double[col];
        }
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                ara[i][j] = 0;
            }
        }
    }
    Matrix() {}
    void print(ofstream &strm)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                strm << fixed << setprecision(2) << ara[i][j] ;
            }
            strm << endl;
        }
    }
    int getRow()
    {
        return row;
    }
    int getCol()
    {
        return col;
    }
    double getElement(int r, int c)
    {
        return ara[r][c];
    }
    void setElement(int r, int c, double val)
    {
        ara[r][c] = val;
    }
    Matrix multiply(Matrix m2)
    {
        if (col != m2.getRow())
        {
            printf("Can't multiply\n");
        }
        int r = row;
        int c = m2.getCol();
        Matrix mul(r, c);

        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {

                double tmp = 0;
                for (int k = 0; k < col; k++)
                {
                    tmp += getElement(i, k) * m2.getElement(k, j);
                }
                mul.setElement(i, j, tmp);
            }
        }
        return mul;
    }
    Matrix addition(Matrix m2)
    {
        if (row != m2.getRow() && col != m2.getCol())
        {
            printf("Can't add\n");
        }
        int r = row;
        int c = col;
        Matrix add(r, c);
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                add.setElement(i, j, getElement(i, j) + m2.getElement(i, j));
            }
        }
        return add;
    }
    Matrix subtraction(Matrix m2)
    {
        if (row != m2.getRow() && col != m2.getCol())
        {
            printf("Can't suntract\n");
        }
        int r = row;
        int c = col;
        Matrix sub(r, c);
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                sub.setElement(i, j, getElement(i, j) - m2.getElement(i, j));
            }
        }
        return sub;
    }
    Matrix getCopy()
    {
        Matrix tmp(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                tmp.setElement(i, j, getElement(i, j));
            }
        }
        return tmp;
    }
    Matrix scaleBy(double value)
    {
        Matrix cpy = getCopy();
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cpy.setElement(i, j, getElement(i, j) * value);
            }
        }
        return cpy;
    }
};
class Point
{
public:
    double x, y, z;
    Point() {}
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    void takeInput()
    {
        cin >> x >> y >> z;
    }
    void print(ofstream &strm)
    {
        strm << fixed << setprecision(7) << x << " " << y << " " << z << endl;
    }
};
class Vector
{

public:
    double x, y, z;
    Vector(Point p1)
    {
        x = p1.x;
        y = p1.y;
        z = p1.z;
    }
    Vector(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector() {}
    void setVector(Point p1)
    {
        x = p1.x;
        y = p1.y;
        z = p1.z;
    }
    double getMagnitude()
    {
        return sqrt(x * x + y * y + z * z);
    }
    double normalize()
    {
        double mag = getMagnitude();
        x /= mag;
        y /= mag;
        z /= mag;
    }
    void print(ofstream &strm)
    {
        strm << x << " " << y << " " << z << endl;
    }
    Point getDirectionPoint()
    {
        return Point(x, y, z);
    }
    Vector addVector(Vector tmp)
    {
        return Vector(x + tmp.x, y + tmp.y, z + tmp.z);
    }
    Vector subVector(Vector tmp)
    {
        return Vector(x - tmp.x, y - tmp.y, z - tmp.z);
    }
    Vector crossProduct(Vector v)
    {
        Point p2 = v.getDirectionPoint();
        Point n, ret;

        n.x = y * p2.z - z * p2.y;
        n.y = z * p2.x - x * p2.z;
        n.z = x * p2.y - y * p2.x;
        return Vector(n);
    }
    Vector scaleBy(double val)
    {
        Vector tmp(x, y, z);
        tmp.x *= val;
        tmp.y *= val;
        tmp.z *= val;
        return tmp;
    }
    double dotProduct(Vector v)
    {
        double ret;
        Point p2 = v.getDirectionPoint();
        ret = x * p2.x + y * p2.y + z * p2.z;
        return ret;
    }
};

Vector Rodri(Vector x, Vector a, double angle)
{
    a.normalize();
    double theta = (PI * angle / 180.0);
    Vector tmp1 = x.scaleBy(cos(theta));
    Vector tmp2 = a.scaleBy(x.dotProduct(a) * (1 - cos(theta)));
    Vector tmp3 = a.crossProduct(x).scaleBy(sin(theta));

    return tmp1.addVector(tmp2).addVector(tmp3);
}

Matrix getIdentityMatrix(int dim)
{
    Matrix tmp(dim, dim);
    for (int i = 0; i < dim; i++)
        tmp.setElement(i, i, 1);
    return tmp;
}
Matrix getTranslateMatrix(double sx, double sy, double sz)
{
    Matrix tmp(4, 4);
    for (int i = 0; i < DIM; i++)
        tmp.setElement(i, i, 1);
    tmp.setElement(0, DIM - 1, sx);
    tmp.setElement(1, DIM - 1, sy);
    tmp.setElement(2, DIM - 1, sz);
    return tmp;
}
double fovY, aspectRatio, near, far;


Point eyePoint, lookPoint, up;

Matrix getViewMatrix()
{

    Vector eye(eyePoint);
    Vector look(lookPoint);

    Vector l = look.subVector(eye);
    l.normalize();
    Vector r = l.crossProduct(Vector(up));
    r.normalize();

    Vector u = r.crossProduct(l);
    Matrix trans = getIdentityMatrix(4);
    trans.setElement(0, 3, -eye.x);
    trans.setElement(1, 3, -eye.y);

    trans.setElement(2, 3, -eye.z);

    Matrix rot(4, 4);

    rot.setElement(0, 0, r.x);
    rot.setElement(0, 1, r.y);
    rot.setElement(0, 2, r.z);

    rot.setElement(1, 0, u.x);
    rot.setElement(1, 1, u.y);
    rot.setElement(1, 2, u.z);

    rot.setElement(2, 0, -l.x);
    rot.setElement(2, 1, -l.y);
    rot.setElement(2, 2, -l.z);

    rot.setElement(3, 3, 1);
    return rot.multiply(trans);
}

Matrix getScaleMatrix(double sx, double sy, double sz)
{
    Matrix tmp(4, 4);
    tmp.setElement(0, 0, sx);
    tmp.setElement(1, 1, sy);
    tmp.setElement(2, 2, sz);
    tmp.setElement(3, 3, 1);

    return tmp;
}

Matrix getRotateMatrix(Vector a, double angle)
{
    Matrix tmp = getIdentityMatrix(4);
    Vector c1 = Rodri(Vector(1, 0, 0), a, angle);
    Vector c2 = Rodri(Vector(0, 1, 0), a, angle);
    Vector c3 = Rodri(Vector(0, 0, 1), a, angle);

    tmp.setElement(0, 0, c1.x);
    tmp.setElement(0, 1, c2.x);
    tmp.setElement(0, 2, c3.x);

    tmp.setElement(1, 0, c1.y);
    tmp.setElement(1, 1, c2.y);
    tmp.setElement(1, 2, c3.y);

    tmp.setElement(2, 0, c1.z);
    tmp.setElement(2, 1, c2.z);
    tmp.setElement(2, 2, c3.z);

    return tmp;
}
Matrix getProjectMatrix()
{
    cin >> fovY >> aspectRatio >> near >> far;

    double fovX = fovY * aspectRatio;
    double theta = PI/ 180.0;
    double t = near * tan(fovY / 2 * theta);
    double r = near * tan(fovX / 2 * theta);

    Matrix tmp(4, 4);
    tmp.setElement(0, 0, near / t);
    tmp.setElement(1, 1, near / t);
    tmp.setElement(2, 2, -(far + near) / (far - near));
    tmp.setElement(2, 3, -(2 * far * near) / (far - near));
    tmp.setElement(3, 2, -1);
    return tmp;
}
void init(stack<Matrix> &state1, stack<Matrix> &state2, stack<Matrix> &state3)
{

    stage1_out.open("stage1.txt");
    stage2_out.open("stage2.txt");
    stage3_out.open("stage3.txt");

    eyePoint.takeInput();
    lookPoint.takeInput();
    up.takeInput();

    state1.push(getIdentityMatrix(4));
    state2.push(getViewMatrix());
    state3.push(getProjectMatrix().multiply(getViewMatrix()));
}

Matrix getPointToMatrix(Point p)
{
    Matrix tmp(4, 1);
    tmp.setElement(0, 0, p.x);
    tmp.setElement(1, 0, p.y);
    tmp.setElement(2, 0, p.z);
    tmp.setElement(3, 0, 1);
    return tmp;
}
Point getMatrixToPoint(Matrix m)
{
    double w = m.getElement(3, 0);
    Point p(m.getElement(0, 0) / w, m.getElement(1, 0) / w, m.getElement(2, 0) / w);
    return p;
}
void transfromTriangle(Matrix top, Point p1, Point p2, Point p3, ofstream &strm)
{

    // cout << p2.x << " " << p2.y << " " << p2.z << endl;
    Matrix m1 = getPointToMatrix(p1);
    Matrix m2 = getPointToMatrix(p2);
    Matrix m3 = getPointToMatrix(p3);

    getMatrixToPoint(top.multiply(m1)).print(strm);
    getMatrixToPoint(top.multiply(m2)).print(strm);
    getMatrixToPoint(top.multiply(m3)).print(strm);
    // top.print(strm);
    strm << endl;
}
void translate(stack<Matrix> &state, Point p)
{

    
    Matrix trans = getTranslateMatrix(p.x, p.y, p.z);
    Matrix applied = state.top().multiply(trans);
    // state.top().print(stage1_out);
    // applied.print(stage1_out);
    state.pop();
    state.push(applied);
}
void rotate(stack<Matrix> &state, Point p, double angle)
{

    
    Matrix trans = getRotateMatrix(Vector(p), angle);
    Matrix applied = state.top().multiply(trans);
    state.pop();
    state.push(applied);
}
void scale(stack<Matrix> &state, Point p)
{
    
    Matrix trans = getScaleMatrix(p.x, p.y, p.z);
    Matrix applied = state.top().multiply(trans);
    state.pop();
    state.push(applied);
    
}
void pushState(stack<Matrix> &state)
{
    state.push(state.top());
}
void popState(stack<Matrix> &state)
{
    state.pop();
}
int main()
{
    freopen("scene.txt", "r", stdin);

    stack<Matrix> states1, states2, states3;

    init(states1, states2, states3);

    string cmd;
    double px, py, pz;

    while (1)
    {
        cin >> cmd;
        if (cmd == "triangle")
        {
            Point p1, p2, p3;

            p1.takeInput();
            p2.takeInput();
            p3.takeInput();
            transfromTriangle(states1.top(), p1, p2, p3, stage1_out);
            transfromTriangle(states2.top(), p1, p2, p3, stage2_out);
            transfromTriangle(states3.top(), p1, p2, p3, stage3_out);
        }
        else if (cmd == "translate")
        {
            Point p;
            p.takeInput();
            translate(states1, p);

            translate(states2, p);
            translate(states3, p);
        }
        else if (cmd == "rotate")
        {
            double angle;
            cin >> angle;
            Point p;
            p.takeInput();
            rotate(states1, p, angle);
            rotate(states2, p, angle);
            rotate(states3, p, angle);
        }
        else if (cmd == "scale")
        {
            Point p;
            p.takeInput();
            scale(states1, p);
            scale(states2, p);
            scale(states3, p);
        }
        else if (cmd == "push")
        {
            pushState(states1);
            pushState(states2);
            pushState(states3);
        }
        else if (cmd == "pop")
        {
            popState(states1);
            popState(states2);
            popState(states3);
        }
        else if (cmd == "end")
        {
            break;
        }
        // stage1_out<<cmd<<endl;
        // states1.top().print(stage1_out);
    }
}