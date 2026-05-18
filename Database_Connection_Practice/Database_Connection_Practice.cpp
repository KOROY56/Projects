// project01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>


#include <mysql/jdbc.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
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
    int table_columns_amount = 0;
    int order_columns_amount = 0;
    int order_vec_col_index = 0;
    int db_col_index = 0;
    std::vector <int> db_row_count;

    std::string order = "";
    std::string order_type = "";
    std::string* special_order_type = &order_type;
    size_t order_type_size = 0; //This variable possesses the amount of character of the order_type
    //e.x. if the order is SELECT then the value of the variable will be 6
    //if the order is ALTER the value of the variable will be 5
    //etc with the rest of the orders


    std::string DefaultDatabase = "";
    std::string OperatingDatabase = "";


    std::vector<std::vector<std::string>> databases;
    std::vector <std::string> col_names = { " " };
    std::vector <std::string> col_type_name = { " " };




public:
    std::vector<std::string> comv;

    DATABASE_HANDLE() {

        std::cout << "Object Created ! " << std::endl;

    }

    ~DATABASE_HANDLE() {
        //delete class_meta_res;
        delete class_res;
        delete class_stmt;
        delete class_con;
        // Note: Do not delete class_driver; the driver lifecycle is managed by the JDBC library.
    }

    //Databases Connection Establishement Functions

    bool driver_creation() {
        try {
            std::cout << "\n\nContacting Driver ..." << std::endl;

            class_driver = sql::mysql::get_mysql_driver_instance();

            std::cout << "Driver Working ! " << std::endl;
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
        }
        catch (sql::SQLException& e) {
            std::cout << "\nError : " << e.what() << std::endl;
            return false;
        }

    }

    bool statement_creation() {
        try {
            std::cout << "\nCreating the MYSQL message ... " << std::endl;

            class_stmt = class_con->createStatement();

            std::cout << "MYSQL message Created ! " << std::endl;
            return true;
        }
        catch (sql::SQLException& e) {
            std::cout << "\nError : " << e.what() << std::endl;
            return false;
        }

    }

    bool result_creation() {
        try {
            class_res = class_stmt->executeQuery(order);
            return true;
        }
        catch (sql::SQLException& e) {
            std::cout << "\nError : " << e.what() << std::endl;
            std::cout << "Error on Line 113" << std::endl;
            return false;
        }
    }

    bool result_extraction() {
        unsigned int i = 0;
        while (class_res->next() < col_names.size()) {
            if (class_res == nullptr) {
                std::cout << "class_res Empty !" << std::endl;
                std::cout << "Error on Line 142" << std::endl;
            }
            else {
                std::cout << class_res->getString(i);
                i++;
                if (i == col_names.size()) {
                    std::cout << "There are not further Data !";
                }
            }
        }
        if (i > 0)
            return true;
        else
            return false;

    }

    bool metadata_creation() {
        try {
            class_meta_res;
            return true;
        }
        catch (sql::SQLException& e) {
            std::cout << "\nError : " << e.what() << std::endl;
            return false;
        }
    }


    bool connection_schema() {
        try {


            std::vector <std::string> vec;

            class_res = class_stmt->executeQuery("SHOW DATABASES;");

            int i = 0;
            while (class_res->next()) {
                vec.push_back(class_res->getString(1));
                i++;
            }

            if (databases.size() < i)
                databases.resize(i);

            //Saving The Amount Of Inner Rows Of The 2D Vector
            db_row_count.push_back(i);

            for (int i = 0; i < db_row_count[db_col_index]; i++) {
                databases[i].push_back(vec[i]);
            }
            std::cout << std::endl;

            for (int i = 0; i < db_row_count[db_col_index]; i++) {
                std::cout << "DB : " << databases[i][db_col_index] << std::endl;
            }




            std::string schema;
            bool run = true;
            bool found = false;
            bool message = true;

            while (run) {

                if (message) {
                    std::cout << "\nChoose a database : ";
                }

                std::getline(std::cin, schema);

                for (int i = 0; i < db_row_count[db_col_index]; i++) {
                    if (schema == databases[i][db_col_index]) {
                        std::cout << "Database : " << databases[i][db_col_index] << " Found ! " << std::endl;
                        run = false;
                        found = true;

                        //Saving the Database Name That The User Chose
                        OperatingDatabase = databases[i][db_col_index];
                    }
                    else {
                        if (i == db_row_count[db_col_index] - 1 && found == false) {
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
            db_col_index++;
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

        for (int i = 1; i <= order_columns_amount; i++) {
            col_names.push_back(class_meta_res->getColumnName(i));
            if (i == order_columns_amount) {
                col_names.push_back("\0");
            }
        }
    }

    void show_column_names() {
        std::cout << "The columns are : " << order_columns_amount << std::endl;
        std::cout << "\n";
        for (int i = 0; i <= col_names.size(); i++) {
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


    void order_col_name_char_count() {

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


        //2nd Method
        //std::transform(col_names.begin(), col_names.end(), col_names.begin(), ::toupper); //This method does not work
                                                                                               //Because a string can not be converted entirely
                                                                                               //into an ascii numerical code data , simply the convertion from string to numbers 
                                                                                               //is not possible only character by character not entirely



        for (int i = order_type_size; i < comv.size() + order_type_size; i++) {
            if (col_names[i] != "\0") {
                if (comv[i + 1] == col_names[i]) {
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


        if (comv.size() != NULL) {
            std::cout << "Next 3 Characters " << std::endl;
            std::cout << "1st : " << (comv[1]) << std::endl;
            std::cout << "2nd : " << (comv[2]) << std::endl;
            std::cout << "3rd : " << (comv[3]) << std::endl;
        }

        else {
            std::cout << "Error on line 389" << std::endl;
        }
    }

    /*void get_col_names_char_count() {
        if (command == "SELECT") {
            for (int i = 0; i <= col_names[i].size(); i++) {
                if (col_names[0][] == )

            }
        }
        else if () {}
        else if () {}
        else if () {}
        else if () {}


    }*/


    /*void get_order_cols_names() {

        std::string colv = class_meta_res->getColumnName();


        colv = class_meta_res->getColumnName();

        for (int i = 0; i <= col_amount; i++) {
            col_names.push_back(*(colv + i));
        }

    }*/


    //Finding Order Type
    void Order_Type() {

        std::string returnvalue;
        std::stringstream ssorder(order);
        std::string command = "";
        ssorder >> command;


        std::cout << "From : " + command << std::endl;
        for (char& com : command) {
            com = std::toupper(static_cast<unsigned char>(com));
        }
        std::cout << "To : " + command << std::endl;


        if (command == "SELECT") {
            std::cout << "Order Found ! " << std::endl;
            order_type = command;
            order_type_size = order_type.size();
        }
        else if (command == "UPDATE") {
            std::cout << "Order Found ! " << std::endl;
            order_type = command;
            order_type_size = order_type.size();
        }
        else if (command == "ALTER") {
            std::cout << "Order Found ! " << std::endl;
            order_type = command;
            order_type_size = order_type.size();
        }
        else if (command == "CREATE") {
            std::cout << "Order Found ! " << std::endl;
            order_type = command;
            order_type_size = order_type.size();
        }
        else if (command == "INSERT") {
            std::cout << "Order Found ! " << std::endl;
            order_type = command;
            order_type_size = order_type.size();
        }
        else if (command == "CONTINUE") {
            std::cout << "\nPlease move on ." << std::endl;
            special_order_type = nullptr;
            order_type_size = NULL;
        }
        else if (command == "END") {
            std::cout << "\nAdios" << std::endl;
            special_order_type = nullptr;
            order_type_size = NULL;
        }
        else {
            std::cout << "Order Not Found ! " << std::endl;
            order_type = "ERROR";
            order_type_size = -1;
        }


    }

    bool order_check(std::string obj_sqlorder) {
        for (char& c : obj_sqlorder) {
            c = std::toupper(static_cast <unsigned char> (c));
        }

        if (obj_sqlorder == "END")
            return false;
        if (obj_sqlorder == "CONTINUE")
            return true;
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
            }
        }
        else {
            std::cout << "Connection Creation Failed." << std::endl;
        }
    }
    else {
        std::cout << "Driver Creation Failed." << std::endl;
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
            std::cout << object1.result_creation() << std::endl;
            std::cout << object1.metadata_creation() << std::endl;
            object1.Order_Type();


            order_type = object1.get_order_type();
            if (order_type != "ERROR") {
                std::cout << order_type << std::endl;
                object1.result_extraction();
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


/*try {
      std::cout << "Program Started ! " << std::endl;

      sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();

      std::cout << "Driver Ok !" << std::endl;


      std::cout << "Connecting..." << std::endl;

      //Creating connection with the sql database

      sql::Connection* con = driver->connect("tcp://127.0.0.1:3306", "root", "150505");

      std::cout << "Connection With Database Established !" << std::endl;


      //Statement Creation
      //or else creating a message type of object so that we can communicate with sql database and server
      sql::Statement* stmt = con->createStatement();

      //Creating a Result
      sql::ResultSet* res = stmt->executeQuery("SHOW DATABASES");

      std::cout << "The Available Schemas To Use : " << std::endl;

      std::cout << "\tDatabases " << std::endl;
      std::cout << "\t--------- " << std::endl;


      //Getting Default Schema of the Database
      std::string DefaultSchema = con->getSchema();

      std::cout << DefaultSchema << std::endl;


      bool run = true;
      std::string schemachoice;

      while (run) {
          if (res->next() != false) {//next function returns either true or false
              //based on the issue , if there is a next row or not
              std::cout << "\t" << res->getString("Database") << std::endl;

          }
          else {

              run = false;
              //By executting this statement it resets the index of the next function
              //and generally the class Result Set along with it's operations data
              res = stmt->executeQuery("SHOW DATABASES");

              bool schemafound = false;



              while (!schemafound) {

                  std::cout << "\nChoose a Schema : ";
                  std::cin >> schemachoice;


                  //Resetting Next() function
                  res->beforeFirst();


                  bool schemaloop;

                  while (schemaloop = res->next()) {

                      if (res->getString(1) == schemachoice) {

                          con->setSchema(schemachoice);

                          //Setting The Schema that we are about to use
                          std::cout << "Schema " << schemachoice << " received ! " << std::endl;
                          schemafound = true;



                      }
                      else {
                          if(!schemafound)
                              std::cout << "SCHEMA NAME NOT MATCHED !" << res->getString(1)<< std::endl;


                      }

                  }

                  if (schemafound == false ) {
                      if (schemaloop == false) {
                          std::cout << "INCORRECT SCHEMA NAME ! " << std::endl;
                          std::cout << "Please Try Again : " << std::endl;
                      }
                      else {


                      }
                  }




              }

          }

      }




      bool terminate = true;
      bool  text_message = true;

      std::string sqlorder;
      std::string checking_order;

      std::cout << "Type an SQL Order : ";

      //The reason that we use .ignore below is because it finds the enter symbol (\n)
      //From the schema Choice name cin and due to the newline character which is enterd
      //Once ENTER key is pressed by the keyboard , it reads an entire new empty line
      //So the Driver Throws an Exception since the function getline finds a new empty line
      std::getline(std::cin.ignore() , sqlorder);


      sql::Statement* order_stmt = con->createStatement();

      sql::ResultSet* order_res = order_stmt->executeQuery(sqlorder);


      sql::ResultSetMetaData* meta_res = order_res->getMetaData();


      while (terminate) {



          if (text_message == false) {
              std::cout << "Type in an SQL Order : " << std::endl;
              std::getline(std::cin >> std::ws, sqlorder);

              order_res = order_stmt->executeQuery(sqlorder);
              meta_res = order_res->getMetaData();

          }


          int cols = meta_res->getColumnCount();
          size_t rows = order_res->rowsCount();
          std::vector<std::string> col_names;


          std::cout << "The columns are : " << cols << std::endl;
          for (int i = 1; i <= cols; i++) {
              col_names.push_back(meta_res->getColumnName(i));

          }

          std::cout << "Column names are : ";
          //In the for Below , due to the
          for (int i = 0; i < col_names.size(); i++) {
              std::cout << col_names[i] << "  ";

          }

          std::cout << "\nCOLUMNS SAVED !!!" << std::endl;


          std::cout << std::endl << "The rows are : " << rows << "\n"  << std::endl;




          //Resetting Next() function
          //order_res->beforeFirst();


          while (order_res->next()) {

              if (sqlorder == " ") {
                  std::cout << "ERROR !" << std::endl;

              }
              else {
                  for (int i = 1; i <= cols; i++) {
                      std::string shell_type = meta_res->getColumnTypeName(i);
                      std::string shell_name = meta_res->getColumnName(i);

                      if (shell_type == "INT" || shell_type == "TINYINT") {
                          std::cout << shell_name + ":" << order_res->getInt(i) << "\t";
                      }
                      else if (shell_type == "VARCHAR" || shell_type == "TEXT") {
                          std::cout << shell_name + ": " << "\t" << order_res->getString(i) << "\t";
                      }
                      else if (shell_type == "DOUBLE" || shell_type == "DECIMAL") {
                          std::cout << shell_name + ": \t" << order_res->getDouble(i);
                      }
                      else {
                          std::cout << "Non Compatible Type !" << std::endl;
                      }


                  }
                  std::cout << " " << std::endl;

              }
          }

          if (text_message) {
              std::cout << "If you wish to continue typing ?"
                  << "\n(Type either end or continue)." << std::endl;

              std::getline(std::cin, sqlorder);

              std::transform(sqlorder.begin(), sqlorder.end(), sqlorder.begin(), ::toupper);

              text_message = false;
          }



          if (sqlorder == "END") {
              terminate = false;

          }


          std::transform(sqlorder.begin(), sqlorder.end(), sqlorder.begin(), ::toupper);
          checking_order = sqlorder;

          if (checking_order == "END") {
              terminate = false;

              //Deleting statement and result objects

          }


          std::cout << "\nHello World!\nGoodbye World !\n";

      }

      delete order_res;
      delete order_stmt;


  }
  catch (sql::SQLException& e) {

      std::cout << "Error : " << e.what() << std::endl;

  }

  std::cout << "Program Terminated Successfully ! " << std::endl;
  */



