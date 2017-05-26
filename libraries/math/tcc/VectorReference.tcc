////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     VectorReference.tcc (math)
//  Authors:  Ofer Dekel
//
////////////////////////////////////////////////////////////////////////////////////////////////////

// utilities
#include "Debug.h"

// stl
#include <algorithm> // for std::generate

namespace ell
{
namespace math
{
    template <typename ElementType, VectorOrientation orientation>
    void VectorReference<ElementType, orientation>::Set(ConstVectorReference<ElementType, orientation> other)
    {
        Operations::Copy(other, *this);
    }

    template<typename ElementType, VectorOrientation orientation>
    template <typename TransformationType>
    void VectorReference<ElementType, orientation>::Set(TransformedConstVectorReference<ElementType, orientation, TransformationType> other)
    {
        const auto& otherVector = other.GetVector();
        auto transformation = other.GetTransformation();

        DEBUG_THROW(_size != otherVector.Size(), utilities::InputException(utilities::InputExceptionErrors::invalidArgument, "this vector and other vector are not the same size."));

        ElementType* pData = _pData;
        const ElementType* pEnd = pData + _increment * _size;
        const ElementType* pOtherData = otherVector.GetDataPointer();
        size_t otherIncrement = otherVector.GetIncrement();

        while (pData < pEnd)
        {
            (*pData) = transformation(*pOtherData);
            pData += _increment;
            pOtherData += otherIncrement;
        }
    }

    template <typename ElementType, VectorOrientation orientation>
    void VectorReference<ElementType, orientation>::Reset()
    {
        Fill(0);
    }

    template <typename ElementType, VectorOrientation orientation>
    void VectorReference<ElementType, orientation>::Fill(ElementType value)
    {
        ElementType* data = _pData;
        ElementType* end = _pData + _size * _increment;

        if (_increment == 1)
        {
            std::fill(data, end, value);
        }
        else
        {
            while (data < end)
            {
                *data = value;
                data += _increment;
            }
        }
    }

    template <typename ElementType, VectorOrientation orientation>
    template <typename GeneratorType>
    void VectorReference<ElementType, orientation>::Generate(GeneratorType generator)
    {
        ElementType* data = _pData;
        ElementType* end = _pData + _size * _increment;

        if (_increment == 1)
        {
            std::generate(data, end, generator);
        }
        else
        {
            while (data < end)
            {
                *data = generator();
                data += _increment;
            }
        }
    }

    template <typename ElementType, VectorOrientation orientation>
    ElementType& VectorReference<ElementType, orientation>::operator[](size_t index)
    {
        DEBUG_THROW(index >= _size, utilities::InputException(utilities::InputExceptionErrors::indexOutOfRange, "index exceeds vector size."));

        return _pData[index * _increment];
    }

    template <typename ElementType, VectorOrientation orientation>
    VectorReference<ElementType, orientation> VectorReference<ElementType, orientation>::GetReference()
    {
        return VectorReference<ElementType, orientation>(_pData, _size, _increment);
    }

    template <typename ElementType, VectorOrientation orientation>
    VectorReference<ElementType, orientation> VectorReference<ElementType, orientation>::GetSubVector(size_t offset, size_t size)
    {
        DEBUG_THROW(offset + size > _size, utilities::InputException(utilities::InputExceptionErrors::indexOutOfRange, "subvector offset + subvector size exceeds vector size."));

        return VectorReference<ElementType, orientation>(_pData + offset * _increment, size, _increment);
    }

    template <typename ElementType, VectorOrientation orientation>
    template <typename TransformationType>
    void VectorReference<ElementType, orientation>::Transform(TransformationType transformation)
    {
        ElementType* current = _pData;
        const ElementType* end = _pData + _size * _increment;
        while (current < end)
        {
            *current = transformation(*current);
            current += _increment;
        }
    }

    template <typename ElementType, VectorOrientation orientation>
    void VectorReference<ElementType, orientation>::operator+=(ConstVectorReference<ElementType, orientation> other)
    {
        Operations::Add(static_cast<ElementType>(1.0), other, *this);
    }

    template <typename ElementType, VectorOrientation orientation>
    template <typename TransformationType>
    void VectorReference<ElementType, orientation>::operator+=(TransformedConstVectorReference<ElementType, orientation, TransformationType> other)
    {
        const auto& otherVector = other.GetVector();
        auto transformation = other.GetTransformation();

        if (_size != otherVector.Size())
        {
            throw utilities::InputException(utilities::InputExceptionErrors::invalidArgument, "this vector and other vector are not the same size.");
        }

        ElementType* pData = _pData;
        const ElementType* pEnd = pData + _increment * _size;
        const ElementType* pOtherData = otherVector.GetDataPointer();
        size_t otherIncrement = otherVector.GetIncrement();

        while (pData < pEnd)
        {
            (*pData) += transformation(*pOtherData);
            pData += _increment;
            pOtherData += otherIncrement;
        }
    }

    template <typename ElementType, VectorOrientation orientation>
    void VectorReference<ElementType, orientation>::operator-=(ConstVectorReference<ElementType, orientation> other)
    {
        Operations::Add(static_cast<ElementType>(-1.0), other, *this);
    }

    template <typename ElementType, VectorOrientation orientation>
    void VectorReference<ElementType, orientation>::operator+=(ElementType value)
    {
        Operations::Add(value, *this);
    }

    template <typename ElementType, VectorOrientation orientation>
    void VectorReference<ElementType, orientation>::operator-=(ElementType value)
    {
        Operations::Add(-value, *this);
    }

    template <typename ElementType, VectorOrientation orientation>
    void VectorReference<ElementType, orientation>::operator*=(ElementType value)
    {
        Operations::Multiply(value, *this);
    }

    template <typename ElementType, VectorOrientation orientation>
    void VectorReference<ElementType, orientation>::operator/=(ElementType value)
    {
        if (value == 0)
        {
            throw utilities::NumericException(utilities::NumericExceptionErrors::divideByZero, "divide by zero");
        }
        Operations::Multiply(static_cast<ElementType>(1.0) / value, *this);
    }
}
}