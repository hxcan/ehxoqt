#ifndef PACOMPAREPICKRESULT_H
#define PACOMPAREPICKRESULT_H

#include "EhxoQtPaPreCompile.h" //QList

#include "PaCompareResult.h" //CompareResult
#include "PaComparator.h" //PaComparator

template<typename T>
class PaComparePickResult
{
private:
    int elementAmount=0; //!<已经累加差值的元素个数。
    quint64 differenceSum=0; //!<差值之和。
    quint64 idealDifference=0; //!<获取理想差值。
    T idealElement; //!<获取理想基准元素。
    bool allEqual=true; //!<是不是大于或等于标准值一侧的列表中全部都是等号。
    QList<T> lessThanValues; //!<小于标准值的值列表．
    static PaComparator<T> * paComparator; //!<比较器。
    QList<T> equalOrGreaterValues; //!<大于或等于标准值的值列表．


public:
    /*!
     * \brief addDifferenceAndCalculateAvarage 累加差值并且计算平均值。平均值即为理想差值。
     * \param difference 此次的差值。
     */
    void addDifferenceAndCalculateAvarage(int difference)
    {
        differenceSum+=difference; //累加。
        elementAmount++; //元素个数累加。
        
        idealDifference=differenceSum/elementAmount; //计算理想差值。
    } //void addDifferenceAndCalculateAvarage(int difference)
    
    /*!
     * \brief appendEqualOrGreaterThanValue 向列表中追加大于或等于的元素．
     * \param value　要追加的元素．
     */
    void appendEqualOrGreaterThanValue(T value)
    {
        equalOrGreaterValues << value;
    } //void appendEqualOrGreaterThanValue(T value)

    /*!
     * \brief appendLessThanValue 向列表中追加小于的元素．
     * \param value　要追加的元素．
     */
    void appendLessThanValue(T value)
    {
        lessThanValues << value;
    } //void appendLessThanValue(T value)

    typedef void result_type;

    /*!
     * \brief pickComparedDataReduce 将比较结果归并到总的结果列表中。
     * \param result 最后归并出来的结果。
     * \param partListedSubDirs 要归并的中间结果对象。
     */
    void operator()(PaComparePickResult &result, const CompareResult<T>  & partListedSubDirs)
    {
        CompareResultRelation compareResultRelation=partListedSubDirs.getCompareResultRelation();

        T value=partListedSubDirs.getValue();

        if (compareResultRelation==LessThan)
        {
            result.appendLessThanValue(value);
        } //if (compareResultRelation==LessThan)
        else
        {
            result.appendEqualOrGreaterThanValue(value);
        } //else
    } //static void pickComparedDataReduce(QList<QList<T>> &result,const CompareResult<T> & partListedSubDirs)

    T compareBase; //!<比较的基准元素。

    PaComparator<T> * getPaComparator() const
    {
        return paComparator;
    }

    void setPaComparator(PaComparator<T> *value)
    {
        paComparator = value;
    }

    T getCompareBase() const
    {
    return compareBase;
    }

    void setCompareBase(const T &value)
    {
    compareBase = value;
    }

    QList<T> getEqualOrGreaterValues() const
    {
        return equalOrGreaterValues;
    }

    void setEqualOrGreaterValues(const QList<T> &value)
    {
        equalOrGreaterValues = value;
    }

    QList<T> getLessThanValues() const
    {
        return lessThanValues;
    }

    void setLessThanValues(const QList<T> &value)
    {
        lessThanValues = value;
    }

    bool getAllEqual() const
    {
    return allEqual;
    }

    void setAllEqual(bool value)
    {
    allEqual = value;
    }

    T getIdealElement() const
    {
    return idealElement;
    }
    
    void setIdealElement(const T &value)
    {
    idealElement = value;
    }
    
    quint64 getIdealDifference() const
    {
    return idealDifference;
    }
    
    void setIdealDifference(int value)
    {
    idealDifference = value;
    }
    
}; //class PaMapReduceFunctions


#endif // MAPREDUCEFUNCTIONS_H





