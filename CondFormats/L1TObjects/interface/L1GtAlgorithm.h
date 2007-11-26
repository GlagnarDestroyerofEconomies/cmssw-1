#ifndef CondFormats_L1TObjects_L1GtAlgorithm_h
#define CondFormats_L1TObjects_L1GtAlgorithm_h

/**
 * \class L1GtAlgorithm
 * 
 * 
 * Description: L1 GT algorithm.  
 *
 * Implementation:
 *    <TODO: enter implementation details>
 *   
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 * 
 * $Date$
 * $Revision$
 *
 */

// system include files
#include <vector>
#include <iostream>

// user include files
//   base class

// forward declarations

// class declaration
class L1GtAlgorithm
{

public:

    /// constructor(s)
    ///   empty
    L1GtAlgorithm();

    ///   name only
    L1GtAlgorithm(const std::string& algoNameValue);

    ///   name and logical expression
    L1GtAlgorithm(const std::string&, const std::string&);

    ///   name, logical expression and bit number
    L1GtAlgorithm(const std::string&, const std::string&, const int);

    /// destructor
    virtual ~L1GtAlgorithm();

public:

    /// get / set algorithm name
    inline const std::string algoName() const
    {
        return m_algoName;
    }

    inline void setAlgoName(const std::string& algoNameValue)
    {
        m_algoName = algoNameValue;
    }

    /// get / set the logical expression for the algorithm
    inline const std::string algoLogicalExpression() const
    {
        return m_algoLogicalExpression;
    }

    inline void setAlgoLogicalExpresssion(const std::string& logicalExpression)
    {
        m_algoLogicalExpression = logicalExpression;
    }

    /// get / set algorithm bit number
    inline const int algoBitNumber() const
    {
        return m_algoBitNumber;
    }

    inline void setAlgoBitNumber(const int algoBitNumberValue)
    {
        m_algoBitNumber = algoBitNumberValue;
    }

public:

    /// get the condition chip number the algorithm is located on
    const int algoChipNr(const int numberConditionChips,
                         const int pinsOnConditionChip,
                         const std::vector<int>& orderConditionChip) const;

    /// get the output pin on the condition chip for the algorithm
    const int algoOutputPin(const int numberConditionChips,
                            const int pinsOnConditionChip,
                            const std::vector<int>& orderConditionChip) const;

    /// print condition
    virtual void print(std::ostream& myCout) const;

private:

    /// algorithm name
    std::string m_algoName;

    /// algorithm logical expresssion
    std::string m_algoLogicalExpression;

    /// bit number (determined by output pin, chip number, chip order)
    /// the result for the algorithm is found at m_algoBitNumber position in
    /// the decision word vector<bool>
    int m_algoBitNumber;

};

#endif /*CondFormats_L1TObjects_L1GtAlgorithm_h*/
