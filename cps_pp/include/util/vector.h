#ifndef INCLUDED_VECTOR_H
#define INCLUDED_VECTOR_H               //!< Prevent multiple inclusion
#include<config.h>
/*!\file
  \brief  Declaration/definition of Vector and Matrix classes.

  Also declarations of functions that perform operations on complex vectors.

  $Id: vector.h,v 1.29.12.6.6.1 2012/09/25 14:18:02 yinnht Exp $
*/
//--------------------------------------------------------------------
//  CVS keywords
//
//  $Author: yinnht $
//  $Date: 2012/09/25 14:18:02 $
//  $Header: /space/cvs/cps/cps++/include/util/vector.h,v 1.29.12.6.6.1 2012/09/25 14:18:02 yinnht Exp $
//  $Id: vector.h,v 1.29.12.6.6.1 2012/09/25 14:18:02 yinnht Exp $
//  $Name: v5_0_16_hantao_io_test_v7 $
//  $Locker:  $
//  $RCSfile: vector.h,v $
//  $Revision: 1.29.12.6.6.1 $
//  $Source: /space/cvs/cps/cps++/include/util/vector.h,v $
//  $State: Exp $
//
//--------------------------------------------------------------------
#include <string.h>
#include <util/data_types.h>
#include <util/vector_asm.h>
CPS_START_NAMESPACE


class Vector; 	// forward declaration
class Matrix;

//------------------------------------------------------------------
// Declarations of some genaral c-style functions that perform
// operations on vectors. For these functions there exists 
// optimized assembly code.
//------------------------------------------------------------------
extern "C" 
{
    //! vector copy; b = a
    void moveMem(void *b, const void *a, int len); 
    void moveFloat(Float *b, const Float *a, int len); 

    //! 3x3 complex matrix multiplication; c = ab 
    void mDotMEqual(IFloat* c, const IFloat* a, const IFloat* b);

    //! 3x3 complex matrix multiplication and sum; c += ab
    void mDotMPlus(IFloat* c, const IFloat* a, const IFloat* b); 

    //! 3x3 complex matrix times vector; y = Mx
    void uDotXEqual(IFloat* y, const IFloat* m, const IFloat* x); 

    //! vector scalar product; a.b
    IFloat dotProduct(const IFloat *a, const IFloat *b, int);

    //! vector addition; a += b
    void vecAddEquVec(IFloat *a, const IFloat *b, int); 	

    //! vector subtraction; a -= b
    void vecMinusEquVec(IFloat *a, const IFloat *b, int);  

    //! vector negation; a = -b
    void vecNegative(IFloat *a, const IFloat *b, int); 	

    //! set all elements to zero
    void vecZero(IFloat *a, int size);

    //! real scalar times vector multiplication; a *= b
    void vecTimesEquFloat(IFloat *a, IFloat b, int); // 

    //! real scalar times vector multiplication; a = c*b
    void vecEqualsVecTimesEquFloat(IFloat *a, IFloat *b, IFloat c, int); // 

    //! vector linear combination; a = bc+d
    void fTimesV1PlusV2(IFloat *a, IFloat b, const IFloat *c,
			const IFloat *d, int size); 	

    //! vector linear combination; a = bc-d
    void fTimesV1MinusV2(IFloat *a, IFloat b, const IFloat *c,
                         const IFloat *d, int size);    

    //! complex vector scalar product; a.b
    void compDotProduct(IFloat *c_r, IFloat *c_i, 
                        const IFloat *a, const IFloat *b, int);

    //! complex vector linear combination; a = bc+d
    void cTimesV1PlusV2(IFloat *a, IFloat b_re, IFloat b_im, const IFloat *c,
                        const IFloat *d, int size);      

    //! Not implemented on qcdsp
    void cTimesV1MinusV2(IFloat *a, IFloat b_re, IFloat b_im, const IFloat *c,
                         const IFloat *d, int size);      // A = b*C-D

    //! matrix linear combination; a = 1-bc
    void oneMinusfTimesMatrix(IFloat *a, IFloat b, const IFloat *c, int n);     

}

//------------------------------------------------------------------
// Declarations of some genaral c-style functions that perform
// operations on vectors. For these functions there is
// no optimized assembly.
//------------------------------------------------------------------

//! Multiplication of complex vector by matrix and addition; y += Mx
void uDotXPlus(IFloat* y, const IFloat* u, const IFloat* x);

//! Multiplication of complex vector by matrix and subtraction; y -= Mx
void uDotXMinus(IFloat* y, const IFloat* u, const IFloat* x);

//! Multiplication of complex vector by hermitian conjugate matrix and summation; y += M^dagger x
void uDagDotXEqual(IFloat* y, const IFloat* u, const IFloat* x);

//! Multiplication of complex vector by hermitian conjugate matrix; y = M^dagger x
void uDagDotXPlus(IFloat* y, const IFloat* u, const IFloat* x);

//------------------------------------------------------------------
// Declarations of some genaral c-style functions that compute
// re/im parts of the su3 characters of various representations
// of su3 matrices.
//------------------------------------------------------------------

extern IFloat reChar6(IFloat *p) ;
extern IFloat imChar6(IFloat *p) ;
extern IFloat reChar8(IFloat *p) ;
extern IFloat reChar10(IFloat *p) ;
extern IFloat imChar10(IFloat *p) ;


//------------------------------------------------------------------
//! The rank of the matrices represented by the Matrix class
//------------------------------------------------------------------
enum{COLORS=3}; 

//------------------------------------------------------------------
//! A class of general 3x3 complex matrices.
//------------------------------------------------------------------
class Matrix
{
    Float u[2*COLORS*COLORS];	// The matrix

    friend class Vector;

  public:
    // CREATORS
    //! General constructor; no initialisation.
    Matrix() {}

    //! Initialisation to real multiple of the unit matrix.
    //------------------------------------------------------------------
    /*!
      The diagonal matrix elements (0,0), (1,1) and (2,2) are set to the real
      number \a c; All other elements are zero.  
      \param c The diagonal matrix element
    */
    Matrix(IFloat c) {*this = c;}

    //! Initialisation to complex multiple of the unit matrix.
    //------------------------------------------------------------------
    /*!
      The diagonal matrix elements (0,0), (1,1) and (2,2) are set to the complex
      number \a c; All other elements are zero.  
      \param c The diagonal matrix element
    */
    Matrix(const Complex& c) {*this = c;}

    //! Copy constructor
    //------------------------------------------------------------------
    /*!
      The matrix is initialised as a copy of the matrix \a m.
      \param m The initialising matrix.
    */
    Matrix(const Matrix& m) {
        memcpy(u, m.u, sizeof(Float) * COLORS * COLORS * 2);
    }

    //! Assignment to real multiple of the unit matrix.
    //------------------------------------------------------------------
    /*!
      The diagonal matrix elements (0,0), (1,1) and (2,2) are set to the real
      number \a c; All other elements are zero.  
      \param c The diagonal matrix element
    */
    Matrix& operator=(IFloat c) {
        this->ZeroMatrix();
        u[0] = u[8] = u[16] = c;
        return *this;
    }

    //! Assignment to complex multiple of the unit matrix.
    //------------------------------------------------------------------
    /*!
      The diagonal matrix elements (0,0), (1,1) and (2,2) are set to the complex
      number \a c; All other elements are zero.  
      \param c The diagonal matrix element
    */
    Matrix& operator=(const Complex& c) {
        this->ZeroMatrix();
        u[0] = u[8] = u[16] = c.real();
        u[1] = u[9] = u[17] = c.imag();
        return *this;
    }

    //! Overloaded assignment
    /*! \a m should not alias this matrix */
    Matrix& operator=(const Matrix& m) {
        if(this != &m) 
            memcpy(u, m.u, sizeof(Float) * COLORS * COLORS * 2);
        return *this;
    }

    // MANIPULATORS
    //! Adds a matrix \a m to this matrix.
    /*!
      \param m The matrix to be added.
      \return The matrix sum.
    */
    Matrix& operator+=(const Matrix& m) {
        for(int i = 0; i < COLORS * COLORS * 2; ++i)
            u[i] += m.u[i];
        return *this;
    }

    //! Adds a real scalar multiple of the unit matrix to this one.
    /*!
      \param c The real scalar multiple
      \return The matrix sum
    */
    Matrix& operator+=(IFloat c)
     { u[0] += c;  u[8] += c;  u[16] += c;  return *this; }

    //! Subtracts a matrix \a m to this matrix.
    /*!
      \param m The matrix to be subtracted.
      \return The matrix difference.
    */
    Matrix& operator-=(const Matrix& m) {
        for(int i = 0; i < COLORS * COLORS * 2; ++i)
            u[i] -= m.u[i];
        return *this;
    }

    //! Subtracts a real scalar multiple of the unit matrix from this one.
    /*!
      \param c The real scalar multiple
      \return The matrix difference
    */
    Matrix& operator-=(IFloat c)
      { u[0] -= c;  u[8] -= c;  u[16] -= c;  return *this; }

    //! Multiplies this matrix by a real scalar.
    /*!
      \param c The real scalar
      \return The multiplied matrix
    */
    Matrix& operator*=(IFloat c) {
        for(int i = 0; i < COLORS * COLORS * 2; ++i)
            u[i] *= c;
        return *this;
    }

    //! Multiplies this matrix by a complex scalar.
    /*!
      \param c The complex scalar
      \return The multiplied matrix
    */
    // Added by Hantao
    Matrix &operator*=(const Complex &c) {
        Complex *uc = (Complex *)u;
        for(int i = 0; i < COLORS * COLORS; ++i) {
            uc[i] *= c;
        }
        return *this;
    }

    Matrix operator+(const Matrix &m)const {
        Matrix tmp(*this);
        tmp += m;
        return tmp;
    }

    Matrix operator-(const Matrix &m)const {
        Matrix tmp(*this);
        tmp -= m;
        return tmp;
    }

    Matrix operator*(const Matrix &m)const {
        Matrix tmp;
        tmp.DotMEqual(*this, m);
        return tmp;
    }

     //! Assignment to matrix product; \a ab
     /*!
         \param a the matrix \a a
	 \param b the matrix \a b
	 \post This matrix is the matrix product \a ab
      */
    void DotMEqual(const Matrix& a, const Matrix& b)
    { mDotMEqual((IFloat *)u, (IFloat *) a.u, (IFloat *) b.u);}

     //! Assignment to matrix product; \a ab
     /*!
         \param a the matrix \a a
	 \param b the matrix \a b
	 \post The matrix product \a ab  is added to  this matrix.
      */
    void DotMPlus(const Matrix& a, const Matrix& b)
    { mDotMPlus((IFloat *)u, (IFloat *)a.u, (IFloat *)b.u);}
       //u += a.u * b.u

    //! Assignment to Matrix transpose.
    void Trans(const IFloat* m);

    //! Assignment to matrix transpose.
    /*!
      \param m A matrix.
      \post This matrix is the transpose of \a m.
      
      \a m must not be an alias of this matrix/
    */
    void Trans(const Matrix& m)
        { Trans((const IFloat *)(m.u)); }

    //! Hermitian conjugate.
    void Dagger(const IFloat* m);

    //! Assignment to hermitian conjugate.
    /*!
      \param m A matrix.
      \post This matrix is the hermitian conjugate of \a m.

      \a a must not be an alias of this matrix
    */
    void Dagger(const Matrix& m)
    	{ Dagger((const IFloat *)&m); }

    //! Not what you might think.
    void TrLessAntiHermMatrix(const Matrix& this_dag);

    //! Assignment to its trace less anti hermitian
    // M <= (M - M^\dag) / 2 - Tr(M - M^\dag)/6
    void TrLessAntiHermMatrix()
    {
        u[0] = u[8] = u[16] = 0.;

        Float tmp = 0.5*(u[2] - u[6]);
        u[2]=tmp; u[6] = -tmp;
        tmp = 0.5*(u[3] + u[7]);
        u[3]=tmp; u[7] = tmp;
        tmp = 0.5*(u[4] - u[12]);
        u[4]=tmp; u[12] = -tmp;
        tmp = 0.5*(u[5] + u[13]);
        u[5]=tmp; u[13] = tmp;
        tmp = 0.5*(u[10] - u[14]);
        u[10]=tmp; u[14] = -tmp;
        tmp = 0.5*(u[11] + u[15]);
        u[11]=tmp; u[15] = tmp;
        
        Float c = 1./3. * (u[1] + u[9] + u[17]);
        u[1] -= c;
        u[9] -= c;
        u[17] -= c;
    }

    //! Assignment to tensor product of vectors.
    void Cross2(const Vector& v1, const Vector& v2);

    //! Assignment to an traceless antihermitian matrix.
    void AntiHermMatrix(const IFloat *a);
    	// a points to an array of 8 real numbers
	    // *this = i \lamda^i * a^i
	    // \lambda^i are 8 Gellmann matrices

    //! Force this matrix to be an SU(3) matrix.
    void Unitarize(void);

    //! Only do the last step in Unitarize.
    void Construct3rdRow(void);

    //! Project this matrix onto SU(3) according to its polar decomposition
    //! Added by Thomas Dumitrescu 06/2006
    int ProjSU3(void);

    //! Assignment to a unit matrix.
    //------------------------------------------------------------------
    /*!
      \post This matrix is a 3x3 unit matrix.
    */
    void UnitMatrix(void) {
        this->ZeroMatrix();
        u[0] = u[8] = u[16] = 1.;
    }

    //! Assignment to a zero matrix.
    //------------------------------------------------------------------
    /*!
      \post This matrix is a 3x3 zero matrix.
    */
    void ZeroMatrix(void) {
        for(int i = 0; i < 18; ++i) {
            u[i] = 0.;
        }
    }

    //! Assignment to a negated matrix.
    /*!
      \param m A matrix.
      \post This matrix has the value \a -m.
    */
    void NegMatrix(const Matrix& m) {
        for(int i = 0; i < COLORS * COLORS * 2; ++i)
            u[i] = -m.u[i];
    }

    //! Assignment to the matrix linear combination 1-xm
    /*!
      \param x A real scalar factor
      \param m A matrix
      \post This matrix has the value 1-xm.
    */
    void OneMinusfTimesM(IFloat x, const Matrix& m)
    	{ oneMinusfTimesMatrix((IFloat *)u, x, (IFloat *)&m,
	  COLORS*COLORS*2); }


    // ACCESSORS
    //! Write access.
    Complex& operator()(int i, int j);
    //! Read access.
    const Complex& operator()(int i, int j) const;

    //! Write access.
    /*!
      \param i A number between 0 and 8
      \return The ([i - i mod 3]/3, i mod 3) matrix element

      Should this method not be private?
    */
    Complex& operator[](int i) { return ((Complex*)u)[i]; }
    //! Read access.
    /*!
      \param i A number between 0 and 8
      \return The ([i - i mod 3]/3, i mod 3) matrix element
    */
    const Complex& operator[](int i) const { return ((Complex*)u)[i]; }
    IFloat elem(int i) { return u[i]; }
    //! Read access.
    /*!
      \param i A number between 0 and 17
      \return  element of the array
    */
    //! The determinant.
    void Det(IFloat* c) const;

    //! Returns the real part of the trace.
    IFloat ReTr() const {
        return u[0] + u[8] + u[16];
    }

    //! Returns the trace.
    Complex Tr() const {
        return ((Complex*)u)[0] + ((Complex*)u)[4] + ((Complex*)u)[8];
    }

    //! -1/2 times the trace of the square.
    IFloat NegHalfTrSquare() const;

    //! The deviation of this matrix from unitarity
    IFloat ErrorSU3() const;

    /*! Returns the SU(3) matrix norm, defined by
      ||X||^2 = -2 trace X^2
    */
    // !!FIXME: Why does it calculate this?
    IFloat norm() const {
        Matrix x2;
        x2.DotMEqual(*this, *this);
        return -2.0*x2.ReTr();
        
        //IFloat *m = (IFloat*)&u[0];
        //return dotProduct(m, m, 18);
    }

    // SU(3) Characters

    Complex Char3() const { return Tr() ; } ;

    Complex Char6() const ;

    Complex Char8() const ;

    Complex Char10() const ;

    void FTimesV1PlusV2(Float fb, Matrix *c, Matrix *d, int len)
//#if TARGET == BGL  || TAGET == QCDOC
#if 0
    { Float coef = fb; vaxpy3_m (u, &coef, c, d, len*3); }
#elif !defined(USE_TEST)
	{ fTimesV1PlusV2((IFloat *)&u, IFloat(fb), (IFloat *)c, 
	                         (IFloat *)d, len*18); }
#else
//#pragma omp parallel for default(shared)
	{ 
		for(int i=0;i<len;i++)
		fTimesV1PlusV2(((IFloat *)u)+18*i, IFloat(fb), (IFloat *)(c+i), 
                        (IFloat *)(d+i), 18); 
	}
#endif


};



//------------------------------------------------------------------
//! A class implementing a general 3 component complex vector.
/*!
  This is a schizophrenic class. It is really designed to be a class
  of complex 3-vectors, and many methods carry out operations on just
  such an object; \e e.g. the overloaded binary operators, the
  matrix-vector multiplications and the normalisation and orthogonalisation
  methods. However, some methods, those with take an argument \c int \a len,
  are really wrappers for functions operating on 1-dimensional floating
  point arrays of any length. They are meant to be used with an array of
  Vectors: the first Vector in the array operates not only on its own data
  but on that of all the other objects in the array by assuming that it is
  at the beginning of a contiguous floating point array. For the sake of
  sanity the argument \a len should be a multiple of 6.
*/
//------------------------------------------------------------------
class Vector
{
    Float v[2*COLORS];	// Vector

    friend class Matrix;

  public:
    // CREATORS
    Vector() {}

    //! Overloaded assignment
    /*! \a x should not alias this matrix */
    Vector& operator=(const Vector& x) {
#if 1
    for(int i=0;i<COLORS*2;i++) v[i] = x.v[i];
#else
    moveMem(v, x.v, COLORS*2*sizeof(Float));
#endif
      return *this; }

    // MANIPULATORS
    //! Multiplies this vector by a real scalar.
    /*!
      \param p The real scalar
      \return The multiplied vector
    */
    Vector& operator*=(IFloat p)
    { vecTimesEquFloat((IFloat *)v, p, COLORS*2); 
    return *this; }

    //! Adds a vector \a m to this vector.
    /*!
      \param m The vector to be added.
      \return The vector sum.
    */
    Vector& operator+=(const Vector& x)
    { vecAddEquVec((IFloat *)v, (IFloat *)x.v, COLORS*2);
      return *this; }

    //! Subtracts a vector \a m to this vector.
    /*!
      \param m The vector to be subtracted.
      \return The vector difference.
    */
    Vector& operator-=(const Vector& x)
    { vecMinusEquVec((IFloat *)v, (IFloat *)x.v, COLORS*2);
      return *this; }

    //! Assignment to matrix-vector product.
    /*!
      \param m A matrix.
      \param x a vector
      \post This vector is takes the value Mx
    */
    void DotXEqual(const Matrix& m, const Vector& x)
    { uDotXEqual((IFloat *)v, (IFloat *) m.u, (IFloat *) x.v); }
       // v = m.u * x.v, m should be in CRAM, x MUST be in DRAM */

    //! Normalisation
    void Normalize(void);

    //! Orthogonalisation
    void Orthogonalize(const Vector& p1);

    //! Zeroing a color vector 
    /*!
      added by Sam 1/9/2006 to implement disconnected F.T
    */
    void Zero()
    {
      for(int i=0; i<2*COLORS; i++) v[i]=0;
    }

    //! simple element access (as a Complex)
    /*!
      added by Sam 1/9/2006 to implement disconnected F.T
    */
    Complex& operator[](int i) 
    {
      return *((Complex*)(v+2*i));
    }

    //--------------------------------------------------------------
    // Functions that act on arrays of vectors of general length.
    // The array of vectors is treated as an array of IFloating 
    // numbers pointed to by &v and having length len.
    // This set of functions does not really fit the way
    // Vector is currently defined (as an array with re/im and 
    // color indeces only. It extends the notion of Vector to
    // a general array of IFloating numbers.
    //--------------------------------------------------------------

    //! Assignment to another vector
    /*!
      \param b Another vector
      \param len The number of real numbers in the vectors.
      \post This vector  = \a b

      \a b should not alias this vector.
    */
    void CopyVec(const Vector *b, int len)
#if 1
    { moveFloat((Float *)&v, (const Float *)b, len); }
#else
    { moveMem(&v, b, len*sizeof(Float)); }
#endif

    //! Square norm.
    /*!
      \param len The number of real numbers in the vectors.
      \return The square norm of this vector.
    */
    Float NormSqNode(int len)
    {return Float( dotProduct((IFloat *)&v, (IFloat *)&v, len) ); }

    //! Square norm with global sum.
    Float NormSqGlbSum(int len);
    Float NormSqGlbSum4D(int len);
    //! Print the vector content to the screen
    void Print(int len);
    
    //! The real part of the dot product
    /*!
      \param b Another vector
      \param len The number of real numbers in the vectors.
      \return The real part of the dot product (v,b).
    */
    Float ReDotProductNode(const Vector *b, int len)
    {return Float( dotProduct((IFloat *)&v, (IFloat *)b, len) ); }

    //! The real part of the dot product with global sum.
    Float ReDotProductGlbSum(const Vector *b, int len);
    Float ReDotProductGlbSum4D(const Vector *b, int len);

    void NormSqArraySliceSum(Float *f_out, const int size, const int dir);
    //!< Sum the square norms of vectors in 3-dim slices.

    void SliceArraySum(Float *sum, const Float *f_in, const int dir);
    //!< Sum an array of Floats on a 4-dim lattice in 3-dim slices.

    void SliceArraySumFive(Float *sum, const Float *f_in, const int dir);
    //!< Sum an array of Floats on a 5-dim lattice in 4-dim slices.

    //! Assign vector to zero.
    /*!
      \param len The number of real numbers in the vectors.
      \post This vector has the value 0.
    */
    void VecZero(int len)
      {vecZero((IFloat*)&v, len);}

    //! Assignment to a negated vector.
    /*!
      \param b A vector.
      \param len The number of real numbers in the vectors.
      \post This vector has the value \a -b.
    */
    void VecNegative(const Vector *b, int len)
	{vecNegative((IFloat *)&v, (IFloat *)b, len);}

    //! Multiplication by a real scalar
    /*!
      \param fb The real scalar
      \param len The number of real numbers in the vectors.
      \post This vector is multiplied by \a fb
    */
    void VecTimesEquFloat(const Float &fb, int len)
    {vecTimesEquFloat((IFloat *)&v, IFloat(fb), len);}

    //! Multiplication by a real scalar
    /*!
      \param u The input vector
      \param fb The real scalar
      \param len The number of real numbers in the vectors.
      \post This vector is multiplied by \a fb
    */
    void VecEqualsVecTimesEquFloat(const Vector *u, const Float &fb, int len)
    {vecEqualsVecTimesEquFloat((IFloat *)&v, (IFloat*)u, IFloat(fb), len);}

    //! Addition of another vector
    /*!
      \param b Another vector
      \param len The number of real numbers in the vectors.
      \post \a b is added to this vector.
    */
    void VecAddEquVec(const Vector *b, int len)
    { vecAddEquVec((IFloat *)&v, (IFloat *)b, len);}

    //! Subtraction of another vector
    /*!
      \param b Another vector
      \param len The number of real numbers in the vectors.
      \post \a b is subtracted from this vector.
    */
    void VecMinusEquVec(const Vector *b, int len)
    { vecMinusEquVec((IFloat *)&v, (IFloat *)b, len);}

    //! Assignment of the linear combination  fb * c + d
    /*!
      \param fb A real scalar
      \param c A vector
      \param d A vector      
      \param len The number of real numbers in the vectors.
      \post \a  This vector takes the value fb * c + d
    */
//    void FTimesV1PlusV2(const Float &fb, const Vector *c,
//			const Vector *d, int len)
     void FTimesV1PlusV2(Float fb, Vector *c, Vector *d, int len)
#if TARGET == BGL  || TAGET == QCDOC
    { Float coef = fb; vaxpy3 ((Vector *)v, &coef, c, d, len/6); }
#else
    { fTimesV1PlusV2((IFloat *)&v, IFloat(fb), (IFloat *)c, 
                        (IFloat *)d, len); }
#endif


    //! Assignment of the linear combination  fb * c - d
    /*!
      \param fb A real scalar
      \param c A vector
      \param d A vector      
      \param len The number of real numbers in the vectors.
      \post  This vector takes the value fb * c - d
    */
    void FTimesV1MinusV2(const Float &fb, const Vector *c,
			 const Vector *d, int len)
    { fTimesV1MinusV2((IFloat *)&v, IFloat(fb), (IFloat *)c, 
                        (IFloat *)d, len); }
    //! The dot product with another vector
    /*!
      \param b Another vector
      \param len The number of real numbers in the vectors.
      \return The dot product of this vector with b.
    */
    Complex CompDotProductNode(const Vector *b, int len)
    {
       IFloat c_r, c_i;
       compDotProduct(&c_r, &c_i, (IFloat *)&v, (IFloat *)b, len);
       return Complex(c_r,c_i);
     }

    //! The dot product with another vector, with global sum
     Complex CompDotProductGlbSum(const Vector *b, int len);
     Complex CompDotProductGlbSum4D(const Vector *b, int len);
  
    //! Assignment of the linear combination  fb * c + d
    /*!
      \param fb A complex scalar
      \param c A vector
      \param d A vector      
      \param len The number of real numbers in the vectors.
      \post  This vector takes the value fb * c + d
    */
    void CTimesV1PlusV2(const Complex &fb, const Vector *c,
                         const Vector *d, int len)
    { cTimesV1PlusV2((IFloat *)&v, real(fb), imag(fb), (IFloat *)c, 
	              (IFloat *)d, len); }
 
    //! Assignment of the linear combination  fb * c - d
    /*!
      \param fb A complex scalar
      \param c A vector
      \param d A vector      
      \param len The number of real numbers in the vectors.
      \post \a  This vector takes the value fb * c - d
    */
    void CTimesV1MinusV2(const Complex &fb, const Vector *c,
                           const Vector *d, int len)
    { cTimesV1MinusV2((IFloat *)&v, real(fb), imag(fb), (IFloat *)c, 
	                (IFloat *)d, len); }

};

CPS_END_NAMESPACE
#endif
