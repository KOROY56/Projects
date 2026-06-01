// Database_Management.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// project01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <vector>

#include <mysql/jdbc.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/sqlstring.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/metadata.h>


class DATABASE_HANDLE {

private:

    sql::mysql::MySQL_Driver* class_driver = nullptr; // = sql::mysql::get_mysql_driver_instance();
    sql::Connection* class_con = nullptr; // = class_driver->connect("127.0.0.1:3306", "root", "150505");
    sql::Statement* class_stmt = nullptr; // class_con->createStatement();
    sql::ResultSet* class_res = nullptr;
    sql::ResultSetMetaData* class_meta_res = nullptr;



    int rows = 0;
    std::vector<int> col_type_name_count; // This is a vector that stores the amount of Columns that there are in a single data base
    // The vector above is meant to provide the max value for the index , when a research is 
    // about to be done for a specific database . 
    // Each Col 
    int table_columns_amount = 0; // It is what it's name says 
    int order_columns_amount = 0; // It is what it's name says


    std::string order = "";
    std::string order_type = "";
    std::string* special_order_type = &order_type;
    size_t order_type_size = 0; //This variable possesses the amount of character of the order_type
    //e.x. if the order is SELECT then the value of the variable will be 6
    //if the order is ALTER the value of the variable will be 5
    //etc with the rest of the orders


    std::string DefaultDatabase = "";
    std::string OperatingDatabase = "";


    int schema_vec_row_index = 0;
    std::vector <std::vector<std::string>> schema_vec;
    std::vector <std::string> col_names = { " " };
    std::vector <std::string> col_type_name = { " " };



public:
    std::vector<std::string> comv;

    DATABASE_HANDLE() {
        std::cout << "Object Created !" << std::endl;
    }

    //Databases Connection Establishement Functions

    bool driver_creation() {
        try {
            std::cout << "\n\nContacting Driver ..." << std::endl;

            class_driver = sql::mysql::get_mysql_driver_instance();

            std::cout << "Driver Working ! " << std::endl;

            return true;
        }
        catch (sql::SQLException& e) {
            std::cout << "\nError : " << e.what() << std::endl;
            return false;
        }


    }

    bool connection_creation() {
        try {

            std::cout << "\nConnecting Server ... " << std::endl;

            class_con = class_driver->connect("tcp://127.0.0.1:3306", "root", "150505");

            std::cout << "Sever Connected  Succesfully ! " << std::endl;

            return true;
        }
        catch (sql::SQLException& e) {
            std::cout << "\nError : " << e.what() << std::endl;
            return false;
        }



    }

    bool statement_creation() {
        try {
            std::cout << "\nCreating MYSQL Statement ... " << std::endl;

            class_stmt = class_con->createStatement();

            std::cout << "MYSQL Statement Created ! " << std::endl;
            return true;

        }
        catch (sql::SQLException& e) {
            std::cout << "\nError : " << e.what() << std::endl;
            return false;
        }

    }

    bool result_creation() {
        try {
            std::string order_t = get_order_type(); // Simply adding calling for order_type , (SELECT,ALTER,CREATE,etc.)
            if (order_t == "SELECT") {
                class_res = class_stmt->executeQuery(order);
                std::cout << "Results Extracted Successfully !" << std::endl;
            }
            else if (order_t == "CREATE") {
                class_stmt->execute(order);
                std::cout << "Create Order Submitted !" << std::endl;
            }
            else if (order_t == "ALTER") {
                class_stmt->execute(order);
                std::cout << "Table Altered !" << std::endl;
            }
            else if (order_t == "UPDATE") {
                class_stmt->executeUpdate(order);
                std::cout << "Table Updated !" << std::endl;
            }
            else if (order_t == "DROP") {
                class_stmt->execute(order);
                std::cout << "Table Dropped !" << std::endl;
            }
            else if (order_t == "INSERT") {
                class_stmt->executeUpdate(order);
                std::cout << "New Lines Inserted !" << std::endl;
            }
            else if (order_t == "SHOW") {
                class_stmt->execute(order);
                std::cout << "Show Order Submitted !" << std::endl;
            }
            else if (order_t == "DELETE") {
                class_stmt->executeUpdate(order);
                std::cout << "Delete order Submitted !" << std::endl;
            }
            else {
                std::cout << "Pass" << std::endl;
            }

            return true;
        }
        catch (sql::SQLException& e) {
            std::cout << "\nError : " << e.what() << std::endl;
            std::cout << "Error on Line 113" << std::endl;
            return false;
        }
    }

    void result_extraction() {

        std::string order_t = get_order_type(); //Get order type function simply passes what order has the user writen , so that we 
        //so that we know what type of order to execute using class_res 
        //Either execute,executequery or executeUpdate
        if (order_t == "SELECT") {
            if (comv[1] != "*") {
                while (class_res->next() != false) {
                    if (class_res == nullptr) {
                        std::cout << "class_res Empty !" << std::endl;
                        std::cout << "Error on Line 142" << std::endl;
                    }
                    else {
                        std::cout << col_names[0] << class_res->getInt(col_names[0]) << " ";
                        std::cout << col_names[1] << class_res->getString(col_names[1]) << std::endl;

                    }
                }
            }
            else {
                //Here the issue is to see what type of data are stored in the columns database
                //If it is needed because we want to appear data that we are not aware what of value are that data
                //in the specific columns , so check ask AI how it works understand how the function works and then
                //You would probably either need to use something general or simply check what type of value the column
                //Has and based on that type of value that can be stored within the column to either call the string function
                //to print data or the int function to print the accordingly data . 
                //Check it out .
                if (col_names.size() != NULL) {
                    class_res->beforeFirst();
                    while (class_res->next()) {}
                    for (int i = 0; i < col_names.size(); i++) {
                        if (col_type_name[i] == "INT" || col_type_name[i] == "INTEGER" || col_type_name[i] == "SMALLINT" || col_type_name[i] == "TINYINT") {
                            std::cout << col_names[i] << " : " << class_res->getInt(col_names[i]) << std::endl;
                        }
                        else if (col_type_name[i] == "BIGINT") {
                            std::cout << col_names[i] << " : " << class_res->getInt64(col_names[i]) << std::endl;
                        }
                        else if (col_type_name[i] == "STRING" || col_type_name[i] == "TEXT" || col_type_name[i] == "CHAR") {
                            std::cout << col_names[i] << "" << class_res->getString(col_names[i]) << std::endl;
                        }
                        else if (col_type_name[i] == "DOUBLE" || col_type_name[i] == "FLOAT" || col_type_name[i] == "DECIMAL" || col_type_name[i] == "NUMERIC") {
                            std::cout << col_names[i] << " : " << class_res->getDouble(col_names[i]) << std::endl;
                        }
                        else if (col_type_name[i] == "BLOB" || col_type_name[i] == "LONGBLOB" || col_type_name[i] == "BINARY" || col_type_name[i] == "VARBINARY") {
                            std::cout << col_names[i] << " : " << class_res->getBlob(col_names[i]) << std::endl;
                        }
                    }
                }
            }
        }
        else if (order_t == "SHOW") {


        }
    }

    bool metadata_creation() {
        try {
            class_meta_res = class_res->getMetaData();
            return true;
        }
        catch (sql::SQLException& e) {
            std::cout << "\nError : " << e.what() << std::endl;
            return false;
        }
    }


    bool connection_schema() {
        try {

            // A 2D vector always starts completely empty , therefore we need to resize the vector to be able to push back data
            // For that we use the resize() function of C++ and we either add 
            schema_vec.resize(schema_vec_row_index + 1);

            class_res = class_stmt->executeQuery("SHOW DATABASES;");

            int i = 0;
            while (class_res->next()) {
                schema_vec[schema_vec_row_index].push_back(class_res->getString(1));
                i++;
            }

            for (int i = 0; i < schema_vec[schema_vec_row_index].size(); i++) {
                std::cout << "DB : " << schema_vec[schema_vec_row_index][i] << std::endl;
            }



            std::string schema_choice;
            bool run = true;
            bool found = false;
            bool message = true;

            while (run) {

                if (message) {
                    std::cout << "\nChoose a database : ";
                }

                std::getline(std::cin, schema_choice);

                for (int i = 0; i < schema_vec[schema_vec_row_index].size(); i++) {
                    if (schema_choice == schema_vec[schema_vec_row_index][i]) {
                        std::cout << "Database : " << schema_vec[schema_vec_row_index][i] << " Found ! " << std::endl;
                        run = false;
                        found = true;

                        //Saving the Database Name That The User Chose
                        OperatingDatabase = schema_vec[schema_vec_row_index][i];
                    }
                    else {
                        if ((i == schema_vec[schema_vec_row_index].size() - 1) && (found == false)) {
                            std::cout << "Database Was Not Found !" << std::endl;
                            std::cout << "Please Try Again : ";
                            message = false;
                        }

                    }

                }
            }

            if (found) {
                class_con->setSchema(OperatingDatabase);

            }



            //Moving on to the next column
            schema_vec_row_index++;

            return true;
        }
        catch (sql::SQLException& e) {
            std::cout << "\nError : " << e.what() << std::endl;
            return false;
        }


    }

    //DATA MAINLY RELATED FUNCTIONS BELOW


    //Setters

    void set_order(std::string neworder) {
        order = neworder;
    }


    //Show Data
    void show_order() {
        std::string order;
        std::cout << order << std::endl;
    }
    void show_order_type() {
        std::string order_type;
        std::cout << order_type << std::endl;
    }


    //Getters
    std::string get_order() {
        if (order != " ") {
            return order;
        }
        else {
            std::cout << "Something went wrong on (line253)! " << std::endl;
            return "ERROR";
        }
    }


    std::string get_order_type() {
        if (order_type != " ") {
            return order_type;
        }
        else {
            std::cout << "Something went wrong (line62)! " << std::endl;
            return "ERROR";
        }
    }


    void get_column_names() {
        //Getting Columns Names To A VECTOR

        for (int i = 0; i < class_meta_res->getColumnCount(); i++) {
            col_names.push_back(class_meta_res->getColumnName(i + 1));
            if (i == order_columns_amount) {
                col_names.push_back("\0"); // The names of the columns are being saved in a private variable so it can be used
                // Globally across the class for easier usage and for safety reasons too
            }
        }
        std::string* returnptr = &col_names[0];

    }



    void show_column_names() {
        std::cout << "The columns are : " << col_names.size() << std::endl;
        std::cout << "\n";
        for (int i = 0; i < col_names.size(); i++) {
            if (col_names[i] != "\0")
                std::cout << col_names[i] << std::endl;
        }
    }

    void get_columns_type_name() {
        int j = 1;
        for (int i = 0; i < col_names.size(); i++) {
            col_type_name.push_back(class_meta_res->getColumnTypeName(j));
            std::cout << "Column Type Saved : " << col_type_name[i] << std::endl;
            j++;
        }

    }


    int order_col_amount() {

    }

    //Finding Referring Columns Within The Order
    std::string* Order_Cols_Names() {



        std::stringstream ssorder(order);
        std::string com;
        std::vector<std::string> returnstring;



        bool terminal_char = false;
        bool extract_sent = NULL;
        int i = 0;

        while (ssorder >> com) {
            if (com != ";") {
                comv.push_back(com);
            }
            else {
                std::cout << "Order Finished." << std::endl;
                terminal_char = true;
                comv.push_back(com);
            }
            if (!terminal_char) {
                std::cout << "There is not END OF LINE character ';' .\nIncorrect input !" << std::endl;
                returnstring.push_back(nullptr);

            }
        }

        if (special_order_type != nullptr) {
            for (std::string& word : comv) {
                for (char& c : word) {
                    c = std::toupper(static_cast<unsigned char>(c));
                }
            }
        }


        if (order_type != "ERROR" && special_order_type != nullptr) {

            std::cout << "\nOrder :\n";

            //Order Shown To The User
            for (int i = 0; i < comv.size(); i++) {
                std::cout << comv[0] << " ";

            }

        }
        else {
            std::cout << "WRONG ORDER ! " << std::endl;
        }

        std::string* ptr = nullptr;
        if (returnstring[0] != " ") {
            ptr = &returnstring[0];
            std::cout << "The Address The First Value is Saved is : " << ptr << std::endl;

        }
        else {
            std::cout << "Error on line 145" << std::endl;
        }

        return ptr;



    }


    void order_col_name_check() {

        //int after_loop_pos = -1;

        //Making CAPS both the vector with the names of the columns 
        //and the columns in the order that writen by the user 
        //1st Method
        //Specifically doing it in caps this way wont generate an error because by calling the function begin() and end()
        //The function expects a single character value , instead it gets an entire string .
        //Implementing the second method would be way to confusing and way more complex
        for (std::string& word : col_names) {
            for (char& c : word) {
                c = std::toupper(static_cast <unsigned char> (c));
            }
        }

        std::cout << "Order Transformed to Caps." << std::endl;

        //2nd Method
        //std::transform(col_names.begin(), col_names.end(), col_names.begin(), ::toupper); //This method does not work
                                                                                               //Because a string can not be converted entirely
                                                                                               //into an ascii numerical code data , simply the convertion from string to numbers 
                                                                                               //is not possible only character by character not entirely


        for (int i = 1; i < comv.size(); i++) {
            if (col_names[i - 1] != "\0") {
                if (comv[i - 1] == col_names[i]) {
                    std::cout << "Character Matched !" << std::endl;
                }
                else if (i != comv.size() - 1) {
                    std::cout << "Typing Error ! " << std::endl;
                    break;
                }
                else {
                    std::cout << "Word Not Matched !" << std::endl;
                    break;
                }
            }
            else {
                std::cout << "Error on line 415";
            }

        }


    }


    //Finding Order Type
    void Order_Type() {

        std::string returnvalue;
        std::stringstream ssorder(order);
        std::string command = "";
        ssorder >> command;


        std::cout << "From : " << command << std::endl;
        for (char& com : command) {
            com = std::toupper(static_cast<unsigned char>(com));
        }
        std::cout << "To : " << command << std::endl;


        if (command == "SELECT") {
            std::cout << "Order Found ! " << std::endl;
            order_type = command;

        }
        else if (command == "UPDATE") {
            std::cout << "Order Found ! " << std::endl;
            order_type = command;

        }
        else if (command == "ALTER") {
            std::cout << "Order Found ! " << std::endl;
            order_type = command;

        }
        else if (command == "CREATE") {
            std::cout << "Order Found ! " << std::endl;
            order_type = command;

        }
        else if (command == "DROP") {
            std::cout << "Order Found !" << std::endl;
            order_type = command;

        }
        else if (command == "INSERT") {
            std::cout << "Order Found ! " << std::endl;
            order_type = command;

        }
        else if (command == "CONTINUE") {
            std::cout << "\nPlease move on ." << std::endl;
            special_order_type = nullptr;

        }
        else if (command == "END") {
            std::cout << "\nAdios" << std::endl;
            special_order_type = nullptr;

        }
        else {
            std::cout << "Order Not Found ! " << std::endl;
            order_type = "ERROR";

        }


    }

    bool order_check(std::string obj_sqlorder) {
        for (char& c : obj_sqlorder) {
            c = std::toupper(static_cast <unsigned char> (c));
        }

        if (obj_sqlorder == "END")
            return true;
        if (obj_sqlorder == "CONTINUE")
            return true;
        return false;
    }


};


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

//MAIN
int main() {

    DATABASE_HANDLE object1;
    bool obj_success = false;


    if (object1.driver_creation()) {
        std::cout << "\nStep 1\n" << std::endl;
        if (object1.connection_creation()) {
            std::cout << "\nStep 2\n" << std::endl;
            if (object1.statement_creation()) {
                std::cout << "\nStep 4\n" << std::endl;
                std::cout << "DATABASE COOPERATING SUCCESSFULLY !" << std::endl;
                obj_success = true;
            }
            else {
                std::cout << "Statement Creation Failed.\n" << std::endl;
                return 0;
            }
        }
        else {
            std::cout << "Connection Establishment Failed." << std::endl;
            return 0;
        }
    }
    else {
        std::cout << "Driver Creation Failed." << std::endl;
        return 0;
    }

    bool text_message = true;

    while (obj_success) {

        object1.connection_schema();

        std::string obj_sqlorder = "";

        bool terminate1 = true;


        std::cout << "Type in an SQL Order : ";

        std::string order_type;
        while (terminate1) {


            if (!text_message) {
                if (!object1.order_check(obj_sqlorder)) {
                    terminate1 = false;
                    obj_success = false;
                    std::cout << "GGS WP BYE !!!" << std::endl;

                }
                else {
                    std::cout << "Type in an SQL Order : ";
                    std::getline(std::cin, obj_sqlorder);
                }
            }
            else {
                if (!terminate1)
                    std::cout << "Type in an SQL Order : ";
                std::getline(std::cin, obj_sqlorder);
            }

            object1.set_order(obj_sqlorder);
            object1.Order_Type();
            object1.order_col_name_check();
            object1.get_column_names();
            object1.get_columns_type_name();
            //object1.

            std::string order_type = object1.get_order_type();

            if (order_type != "ERROR") {
                std::cout << "Order Type : " << order_type << std::endl;

                std::cout << object1.result_creation() << std::endl;
                std::cout << object1.metadata_creation() << std::endl;

                object1.result_extraction(); // Data reveal , order output.
            }
            else {
                std::cout << "Wrong Order !" << std::endl;
            }

            if (text_message) {
                std::cout << "If you want to continue typing " << std::endl;
                std::cout << "Type continue or type end to stop." << std::endl;
                std::getline(std::cin.ignore(), obj_sqlorder);
                text_message = false;
            }
        }

    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
