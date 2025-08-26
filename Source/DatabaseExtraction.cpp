#include <iostream>
#define NOMINMAX 
#include <windows.h>
#include <sqlext.h>
#include "QuizManager.h"

using namespace std;

void checkError(SQLRETURN ret, SQLHANDLE handle, SQLSMALLINT type, std::string message);
string toString(SQLWCHAR* buffer);


void QuizManager::load_Questions_Answers()
{
    answers.clear();
    questions.clear();

    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    SQLRETURN ret;

    // Allocate environment handle
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    checkError(ret, hEnv, SQL_HANDLE_ENV, "Allocating Environment Handle");

    // Set ODBC version
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    checkError(ret, hEnv, SQL_HANDLE_ENV, "Setting ODBC Version");

    // Allocate connection handle
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    checkError(ret, hDbc, SQL_HANDLE_DBC, "Allocating Connection Handle");

    // Connect to the Access database (DSN must be configured in ODBC settings)
    SQLCHAR connectionString[] =
        "DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=Resources\\Database\\Fractured_in_Time.accdb";


    ret = SQLDriverConnectA(hDbc, NULL, connectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    checkError(ret, hDbc, SQL_HANDLE_DBC, "Connecting to Database");

    std::cout << "Connected to Access database!" << std::endl;

    // Allocate statement handle
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(ret, hStmt, SQL_HANDLE_STMT, "Allocating Statement Handle");

    // Question table extraction

    //SQL retrieve to data from Question table
    ret = SQLExecDirectA(hStmt, (SQLCHAR*)"SELECT * FROM Question;", SQL_NTS);
    checkError(ret, hStmt, SQL_HANDLE_STMT, "Executing SQL Query");

    //SQL variables to pull from Question Table
    SQLINTEGER questionID;
    SQLWCHAR question_Text[1024];
    SQLINTEGER hasImage; //SQL variables do not have a Yes/No type -> retrieved as integer
    // Yes -> 1, No -> 0
    SQLDOUBLE timeLimit;


    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &questionID, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_CHAR, question_Text, sizeof(question_Text), NULL);
        SQLGetData(hStmt, 3, SQL_C_LONG, &hasImage, 0, NULL);
        SQLGetData(hStmt, 4, SQL_C_DOUBLE, &timeLimit, 0, NULL);

        //Storing in the appropriate data structures (with conversions)
        questions.push_back(Question(questionID, toString(question_Text), (hasImage != 0), timeLimit));
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);  // Free the previous statement
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);  // Allocate a new one
    checkError(ret, hStmt, SQL_HANDLE_STMT, "Reallocating Statement Handle");


    //Answer table extraction

    ret = SQLExecDirectA(hStmt, (SQLCHAR*)"SELECT * FROM Answer;", SQL_NTS);
    checkError(ret, hStmt, SQL_HANDLE_STMT, "Executing SQL Query");


    //Variables to pull from Answer table
    SQLINTEGER answerID;
    SQLINTEGER fromQuestionID;
    SQLINTEGER toQuestionID;
    SQLINTEGER correctness;
    SQLWCHAR answer_Text[512];
    SQLINTEGER inventory_AnswerID;
    SQLINTEGER requiresInventory;
    SQLINTEGER removedAfterPassed;

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_LONG, &answerID, 0, NULL);
        SQLGetData(hStmt, 2, SQL_C_LONG, &fromQuestionID, 0, NULL);
        SQLGetData(hStmt, 3, SQL_C_LONG, &toQuestionID, 0, NULL);
        SQLGetData(hStmt, 4, SQL_C_LONG, &correctness, 0, NULL);
        SQLGetData(hStmt, 5, SQL_C_CHAR, answer_Text, sizeof(answer_Text), NULL);
        SQLGetData(hStmt, 6, SQL_C_LONG, &inventory_AnswerID, 0, NULL);
        SQLGetData(hStmt, 7, SQL_C_LONG, &requiresInventory, 0, NULL);
        SQLGetData(hStmt, 8, SQL_C_LONG, &removedAfterPassed, 0, NULL);

        answers.push_back(Answer(answerID, fromQuestionID, toQuestionID, correctness, toString(answer_Text),
            inventory_AnswerID, (requiresInventory != 0), (removedAfterPassed != 0)));
    }


    // Cleanup
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    cout << "Questions and answers successfully loaded" << endl;

}


void checkError(SQLRETURN ret, SQLHANDLE handle, SQLSMALLINT type, std::string message) {
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        SQLWCHAR sqlState[6], messageText[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT textLength;
        SQLGetDiagRec(type, handle, 1, sqlState, &nativeError, messageText, sizeof(messageText), &textLength);
        //std::cerr << "ODBC ERROR: " << message << "\nSQLSTATE: " << sqlState << "\nMessage: " << messageText << std::endl;
        exit(1);
    }
}

string toString(SQLWCHAR* buffer) {
    return std::string(reinterpret_cast<char*>(buffer));
}