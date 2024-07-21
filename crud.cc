#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "cpp_crud_toko";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Connected to the database successfully." << endl;
        } else {
            cerr << "Connection failed: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init failed" << endl;
    }
    return conn;
}

void create_product(const string& name, const string& category, double price, int stock) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "INSERT INTO products (name, category, price, stock) VALUES ('" << name << "', '" << category << "', " << price << ", " << stock << ")";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Product successfully added." << endl;
        }
        mysql_close(conn);
    }
}

void get_products() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM products")) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Name: " << row[1] << ", Category: " << row[2] << ", Price: " << row[3] << ", Stock: " << row[4] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void update_product(int product_id, const string& name, const string& category, double price, int stock) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE products SET name = '" << name << "', category = '" << category << "', price = " << price << ", stock = " << stock << " WHERE id = " << product_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Product successfully updated." << endl;
        }
        mysql_close(conn);
    }
}

void delete_product(int product_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM products WHERE id = " << product_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Product successfully deleted." << endl;
        }
        mysql_close(conn);
    }
}

int main() {
    int role_choice;
    cout << "Select Role:\n";
    cout << "1. Admin\n";
    cout << "2. User\n";
    cout << "Enter choice: ";
    cin >> role_choice;

    int choice;
    while (true) {
        if (role_choice == 1) {
            cout << "\nAdmin Menu:\n";
            cout << "1. Add Product\n";
            cout << "2. Show All Products\n";
            cout << "3. Update Product\n";
            cout << "4. Delete Product\n";
            cout << "5. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) {
                string name, category;
                double price;
                int stock;
                cout << "Enter product name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter category: ";
                getline(cin, category);
                cout << "Enter price: ";
                cin >> price;
                cout << "Enter stock quantity: ";
                cin >> stock;
                create_product(name, category, price, stock);
            } else if (choice == 2) {
                get_products();
            } else if (choice == 3) {
                int product_id;
                string name, category;
                double price;
                int stock;
                cout << "Enter product ID to update: ";
                cin >> product_id;
                cin.ignore();
                cout << "Enter new product name: ";
                getline(cin, name);
                cout << "Enter new category: ";
                getline(cin, category);
                cout << "Enter new price: ";
                cin >> price;
                cout << "Enter new stock quantity: ";
                cin >> stock;
                update_product(product_id, name, category, price, stock);
            } else if (choice == 4) {
                int product_id;
                cout << "Enter product ID to delete: ";
                cin >> product_id;
                delete_product(product_id);
            } else if (choice == 5) {
                break;
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else if (role_choice == 2) {
            cout << "\nUser Menu:\n";
            cout << "1. Show All Products\n";
            cout << "2. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) {
                get_products();
            } else if (choice == 2) {
                break;
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else {
            cout << "Invalid role choice. Exiting..." << endl;
            break;
        }
    }
    return 0;
}