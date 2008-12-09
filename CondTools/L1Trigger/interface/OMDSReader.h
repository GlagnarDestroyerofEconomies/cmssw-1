#ifndef CondTools_L1Trigger_OMDSReader_h
#define CondTools_L1Trigger_OMDSReader_h
// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     OMDSReader
// 
/**\class OMDSReader OMDSReader.h CondTools/L1Trigger/interface/OMDSReader.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  Werner Sun
//         Created:  Sun Mar  2 01:36:06 CET 2008
// $Id: OMDSReader.h,v 1.9 2008/12/05 19:20:22 wsun Exp $
//

// system include files
#include <memory>
#include "boost/shared_ptr.hpp"

// user include files
#include "CondCore/DBCommon/interface/CoralTransaction.h"
#include "CondTools/L1Trigger/interface/DataManager.h"
#include "RelationalAccess/IQuery.h"
#include "CoralBase/AttributeList.h"
#include "CoralBase/AttributeSpecification.h"
#include "CoralBase/Attribute.h"

#include "RelationalAccess/ITable.h"
#include "RelationalAccess/ISchema.h"
#include "RelationalAccess/ISessionProxy.h"
#include "RelationalAccess/ICursor.h"

// forward declarations

namespace l1t
{

  class OMDSReader : public DataManager
  {

  public:
    // std::vector< std::string > is the list of attribute names.
    // We need to store the column names because there is no way to ask
    // AttributeList for this information.  We have a vector of AttributeLists
    // because the query may return more than one row, each of which is
    // encapsulated in an AttributeList.
    class QueryResults
	{
	public:
	  QueryResults() {}
	  QueryResults( const std::vector< std::string >& columnNames,
			const std::vector< coral::AttributeList >& attLists )
	    : m_columnNames( columnNames), m_attributeLists( attLists ) {}

	  virtual ~QueryResults() {}

	  const std::vector< std::string >& columnNames() const
	    { return m_columnNames ; }
	  const std::vector< coral::AttributeList >& attributeLists() const
	    { return m_attributeLists ; }
	  bool queryFailed() const { return m_attributeLists.size() == 0 ; }
	  int numberRows() const { return m_attributeLists.size() ; }

	  // Return value is false if variable is null.
	  template< class T >
	    bool fillVariable( const std::string& columnName,
			       T& outputVariable ) const ;

	  template< class T >
	    bool fillVariableFromRow( const std::string& columnName,
				      int rowNumber,
				      T& outputVariable ) const ;

	  // If there is only one column, no need to give column name
	  template< class T >
	    bool fillVariable( T& outputVariable ) const ;

	  template< class T >
	    bool fillVariableFromRow( int rowNumber,
				      T& outputVariable ) const ;

	private:
	  std::vector< std::string > m_columnNames ;
	  std::vector< coral::AttributeList > m_attributeLists ;
	} ;

    OMDSReader( const std::string& connectString,
		const std::string& authenticationPath ) ;

    virtual ~OMDSReader();

      // ---------- const member functions ---------------------

      // These functions encapsulate basic SQL queries of the form
      //
      // SELECT <columns> FROM <schema.table> WHERE <conditionLHS> = <conditionRHS>
      //
      // where
      //
      // <columns> can be one or many column names
      // <conditionRHS> can be a string or the result of another query

      // Assume data type of condition RHS is std::string
      const QueryResults basicQuery(
	const std::vector< std::string >& columnNames,
	const std::string& schemaName, // for nominal schema, use ""
	const std::string& tableName,
	const std::string& conditionLHS = "",
	const QueryResults conditionRHS = QueryResults(),
	                                           // must have only one row
	const std::string& conditionRHSName = ""
	                 // if empty, conditionRHS must have only one column
	) const ;

      // Assume data type of condition RHS is std::string
      const QueryResults basicQuery(
	const std::string& columnName,
	const std::string& schemaName, // for nominal schema, use ""
	const std::string& tableName,
	const std::string& conditionLHS = "",
	const QueryResults conditionRHS = QueryResults(),
	                                           // must have only one row
	const std::string& conditionRHSName = ""
	                 // if empty, conditionRHS must have only one column
	) const ;

      // For any data type of condition RHS.  Input dummyVariable is used
      // to determine type.
      template< class T >
	const QueryResults basicQueryGenericKey(
	  const std::vector< std::string >& columnNames,
	  const std::string& schemaName, // for nominal schema, use ""
	  const std::string& tableName,
	  const std::string& conditionLHS = "",
	  const QueryResults conditionRHS = QueryResults(),
	                                            // must have only one row
	  const std::string& conditionRHSName = ""
	                    // if empty, conditionRHS must have only one column
	  ) const ;

      // For any data type of condition RHS.  Input dummyVariable is used
      // to determine type.
      template< class T >
	const QueryResults basicQueryGenericKey(
	  const std::string& columnName,
	  const std::string& schemaName, // for nominal schema, use ""
	  const std::string& tableName,
	  const std::string& conditionLHS = "",
	  const QueryResults conditionRHS = QueryResults(),
	                                           // must have only one row
	  const std::string& conditionRHSName = ""
	                 // if empty, conditionRHS must have only one column
	  ) const ;

      // For any data type of condition RHS.  Input dummyVariable is used
      // to determine type.
      template< class T >
	const QueryResults basicQuery(
	  const std::vector< std::string >& columnNames,
	  const std::string& schemaName, // for nominal schema, use ""
	  const std::string& tableName,
	  const std::string& conditionLHS,
	  const QueryResults conditionRHS, // must have only one row
	  const std::string& conditionRHSName,
	                    // if empty, conditionRHS must have only one column
	  T& dummyVariable  // determines C++ type of condition RHS data
	  ) const ;

      // For any data type of condition RHS.  Input dummyVariable is used
      // to determine type.
      template< class T >
	const QueryResults basicQuery(
	  const std::string& columnName,
	  const std::string& schemaName, // for nominal schema, use ""
	  const std::string& tableName,
	  const std::string& conditionLHS,
	  const QueryResults conditionRHS,
	                                           // must have only one row
	  const std::string& conditionRHSName,
	                 // if empty, conditionRHS must have only one column
	  T& dummyVariable // determines C++ type of condition RHS data
	  ) const ;

      template< class T >
	const QueryResults singleAttribute( const T& data ) const ;

      std::vector< std::string > columnNames(
	const std::string& schemaName, // for nominal schema, use ""
	const std::string& tableName ) const ;

      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------

   private:
      OMDSReader(const OMDSReader&); // stop default

      const OMDSReader& operator=(const OMDSReader&); // stop default

      // ---------- member data --------------------------------
      cond::CoralTransaction* m_coralTransaction ;
};

  template< class T > const OMDSReader::QueryResults
  OMDSReader::basicQueryGenericKey(
    const std::vector< std::string >& columnNames,
    const std::string& schemaName,
    const std::string& tableName,
    const std::string& conditionLHS,
    const QueryResults conditionRHS,
    const std::string& conditionRHSName ) const
  {
    coral::ISchema& schema = schemaName.empty() ?
      m_coralTransaction->nominalSchema() :
      m_coralTransaction->coralSessionProxy().schema( schemaName ) ;

    coral::ITable& table = schema.tableHandle( tableName ) ;

    // Pointer is deleted automatically at end of function.
    boost::shared_ptr< coral::IQuery > query( table.newQuery() ) ;

    // Construct query
    std::vector< std::string >::const_iterator it = columnNames.begin() ;
    std::vector< std::string >::const_iterator end = columnNames.end() ;
    for( ; it != end ; ++it )
      {
	query->addToOutputList( *it ) ;
      }

    // Only apply condition if RHS has one row.
    if( !conditionLHS.empty() && conditionRHS.numberRows() == 1 )
      {
	if( !conditionRHSName.empty() )
	  {
	    // Use type of dummyVariable to determine type of condition RHS
	    coral::AttributeList attList ;
	    attList.extend( conditionRHSName, typeid( T ) ) ;
	    T tmp ;
	    conditionRHS.fillVariable( conditionRHSName, tmp ) ;
	    attList[ conditionRHSName ].data< T >() = tmp ;

	    query->setCondition( conditionLHS + " = :" + conditionRHSName,
				 attList ) ;
	  }
	else if( conditionRHS.columnNames().size() == 1 )
	  // check for only one column
	  {
	    query->setCondition( conditionLHS + " = :" +
				   conditionRHS.columnNames().front(),
				 conditionRHS.attributeLists().front() ) ;
	  }
      }

    coral::ICursor& cursor = query->execute() ;

    // Copy AttributeLists for external use because the cursor is deleted
    // when the query goes out of scope.
    std::vector< coral::AttributeList > atts ;
    while( cursor.next() )
      {
	atts.push_back( cursor.currentRow() ) ;
      } ;

    return QueryResults( columnNames, atts ) ;
  }

  template< class T> const OMDSReader::QueryResults
  OMDSReader::basicQueryGenericKey(
    const std::string& columnName,
    const std::string& schemaName,
    const std::string& tableName,
    const std::string& conditionLHS,
    const QueryResults conditionRHS,
    const std::string& conditionRHSName ) const
  {
    std::vector< std::string > columnNames ;
    columnNames.push_back( columnName ) ;
    return basicQueryGenericKey( columnNames, schemaName, tableName,
				 conditionLHS, conditionRHS, conditionRHSName);
  }

  template< class T > const OMDSReader::QueryResults
  OMDSReader::basicQuery(
    const std::vector< std::string >& columnNames,
    const std::string& schemaName,
    const std::string& tableName,
    const std::string& conditionLHS,
    const QueryResults conditionRHS,
    const std::string& conditionRHSName,
    T& dummyVariable ) const
  {
    coral::ISchema& schema = schemaName.empty() ?
      m_coralTransaction->nominalSchema() :
      m_coralTransaction->coralSessionProxy().schema( schemaName ) ;

    coral::ITable& table = schema.tableHandle( tableName ) ;

    // Pointer is deleted automatically at end of function.
    boost::shared_ptr< coral::IQuery > query( table.newQuery() ) ;

    // Construct query
    std::vector< std::string >::const_iterator it = columnNames.begin() ;
    std::vector< std::string >::const_iterator end = columnNames.end() ;
    for( ; it != end ; ++it )
      {
	query->addToOutputList( *it ) ;
      }

    // Only apply condition if RHS has one row.
    if( !conditionLHS.empty() && conditionRHS.numberRows() == 1 )
      {
	if( !conditionRHSName.empty() )
	  {
	    // Use type of dummyVariable to determine type of condition RHS
	    coral::AttributeList attList ;
	    attList.extend( conditionRHSName, typeid( T ) ) ;
	    conditionRHS.fillVariable( conditionRHSName, dummyVariable ) ;
	    attList[ conditionRHSName ].data< T >() = dummyVariable ;

	    query->setCondition( conditionLHS + " = :" + conditionRHSName,
				 attList ) ;
	  }
	else if( conditionRHS.columnNames().size() == 1 )
	  // check for only one column
	  {
	    query->setCondition( conditionLHS + " = :" +
				   conditionRHS.columnNames().front(),
				 conditionRHS.attributeLists().front() ) ;
	  }
      }

    coral::ICursor& cursor = query->execute() ;

    // Copy AttributeLists for external use because the cursor is deleted
    // when the query goes out of scope.
    std::vector< coral::AttributeList > atts ;
    while( cursor.next() )
      {
	atts.push_back( cursor.currentRow() ) ;
      } ;

    return QueryResults( columnNames, atts ) ;
  }

  template< class T> const OMDSReader::QueryResults
  OMDSReader::basicQuery(
    const std::string& columnName,
    const std::string& schemaName,
    const std::string& tableName,
    const std::string& conditionLHS,
    const QueryResults conditionRHS,
    const std::string& conditionRHSName,
    T& dummyVariable ) const
  {
    std::vector< std::string > columnNames ;
    columnNames.push_back( columnName ) ;
    return basicQuery( columnNames, schemaName, tableName,
		       conditionLHS, conditionRHS, conditionRHSName,
		       dummyVariable ) ;
  }

  template< class T > const OMDSReader::QueryResults
  OMDSReader::singleAttribute( const T& data ) const
  {
    std::vector< std::string > names ;
    names.push_back( "dummy" ) ;

    coral::AttributeList attList ;
    attList.extend( "dummy", typeid( T ) ) ;
    attList[ "dummy" ].data< T >() = data ;

    std::vector< coral::AttributeList > atts ;
    atts.push_back( attList ) ;

    return QueryResults( names, atts ) ;
  }

  template< class T > bool
    OMDSReader::QueryResults::fillVariable(
      const std::string& columnName,
      T& outputVariable ) const
    {
      return fillVariableFromRow( columnName, 0, outputVariable ) ;
    }

  template< class T > bool
    OMDSReader::QueryResults::fillVariableFromRow(
      const std::string& columnName,
      int rowNumber,
      T& outputVariable ) const
    {
      // Check index in bounds
      if( rowNumber < 0 || rowNumber >= numberRows() ) return false ;
      const coral::AttributeList& row = m_attributeLists[ rowNumber ] ;
      if( row[ columnName ].isNull() ) return false ;
      outputVariable = row[ columnName ].template data< T >() ;
      return true ;
    }

  template< class T > bool
    OMDSReader::QueryResults::fillVariable( T& outputVariable ) const
    {
      return fillVariableFromRow( 0, outputVariable ) ;
    }

  template< class T > bool
    OMDSReader::QueryResults::fillVariableFromRow( int rowNumber,
						   T& outputVariable ) const
    {
      // Check index in bounds and only one column
      if( rowNumber < 0 || rowNumber >= numberRows() ||
	  m_columnNames.size() != 1 ) return false ;
      const coral::AttributeList& row = m_attributeLists[ rowNumber ] ;
      if( row[ m_columnNames.front() ].isNull() ) return false ;
      outputVariable = row[ m_columnNames.front() ].template data< T >() ;
      return true ;
    }

}
#endif
