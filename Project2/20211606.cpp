#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include "mysql.h"
#include <string>
#include <fstream>
#include <string.h>
#pragma comment(lib, "libmysql.lib")
#define MAXLINE 2048
using namespace std;
const char* host = "localhost";
const char* user = "root";
const char* pw = "hy09082430@";
const char* db = "project2";

int main(void) {

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	ifstream rfp;
	const char* query;
	int type;
	int flag = 0, state;
	
	if (mysql_init(&conn) == NULL)
		cout << "mysql_init() error!";

	// mysql connection
	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		cout << mysql_errno(&conn) << " ERROR : " << mysql_error(&conn) << endl;
		return 1;
	}

	else {
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			cout << mysql_errno(&conn) << " ERROR : " << mysql_error(&conn) << endl;
			return 0;
		}

		// create table and insert values
		rfp.open("20211606_create.txt");
		if (rfp.fail()) {
			cout << "file open error\n";
			return 0;
		}
		string str;
		while (getline(rfp, str)) {
			query = str.c_str();
			mysql_query(connection, query);
		}
		rfp.close();

		// query start
		while (1) {
			cout << "------- SELECT QUERY TYPES -------\n\n";
			cout << "\t1. TYPE グ\n";
			cout << "\t2. TYPE ケ\n";
			cout << "\t3. TYPE ゲ\n";
			cout << "\t4. TYPE コ\n";
			cout << "\t5. TYPE ゴ\n";
			cout << "\t0. QUIT\n\n";
			cout << "Select a number : ";
			cin >> type;
			int one_flag = 0;
			switch (type) {
			// query 1
			case 1:
			{
				int t_ID;
				cout << "Assume truck X is destroyed in a crash.\n";
				cout << "Input truck X number: ";
				cin >> t_ID;
				while (1) {
					int subtype;
					cout << "\n\n----- Subtypes in TYPE グ -----\n";
					cout << "\t1. TYPE グ-1.\n";
					cout << "\t2. TYPE グ-2.\n";
					cout << "\t3. TYPE グ-3.\n";
					cout << "\t0. QUIT\n\n";
					cout << "Select a number : ";
					cin >> subtype;
					switch (subtype) {
					case 1:
					{
						cout << "\n\n---- TYPE グ-1 ----\n\n";
						cout << "** Find all customers who had a package on the truck at the time of the crash. **\n";
						char Query1_1_1[MAXLINE] = "select D.customer_id from delivery D where D.vehicle_id = ";
						char Query1_1_2[MAXLINE] = " and D.state_code = 777 and D.delivery_date is null";
						char Query1_1[MAXLINE];
						sprintf(Query1_1, "%s%d%s", Query1_1_1, t_ID, Query1_1_2);

						state = mysql_query(connection, Query1_1);
						if (!state) {
							sql_result = mysql_store_result(connection);
							cout << "Customer ID in crashed truck : ";
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
								cout << sql_row[0] << ' ';
							mysql_free_result(sql_result);
							cout << endl;
							cout << endl;
						}
						continue;
					}
						
					case 2:
					{
						cout << "\n\n---- TYPE グ-2 ----\n\n";
						cout << "** Find all recipients who had a package on that truck at the time of the crash. **\n";
						char Query1_2_1[MAXLINE] = "select recipient from delivery where vehicle_id = ";
						char Query1_2_2[MAXLINE] = " and state_code = 777 and delivery_date is null";
						char Query1_2[MAXLINE];
						sprintf(Query1_2, "%s%d%s", Query1_2_1, t_ID, Query1_2_2);
						cout << Query1_2 << '\n';
						state = mysql_query(connection, Query1_2);
						if (!state) {
							sql_result = mysql_store_result(connection);
							cout << "Recipients name in crashed truck : ";
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
								cout << sql_row[0] << ' ';
							mysql_free_result(sql_result);
							cout << endl;
							cout << endl;
						}
						continue;
					}
						
					case 3:
					{
						cout << "\n\n---- TYPE グ-3 ----\n\n";
						cout << "** Find the last successful delivery by that truck prior to the crash. **\n";
						char Query1_3_1[MAXLINE] = "select D.delivery_id from delivery D where state_code != 777 and delivery_id = (select delivery_id from delivery where state_code != 777 and vehicle_id = ";
						char Query1_3_2[MAXLINE] = " and delivery_date is not null order by delivery_date DESC LIMIT 1)";
						char Query1_3[MAXLINE];
						sprintf(Query1_3, "%s%d%s", Query1_3_1, t_ID, Query1_3_2);

						state = mysql_query(connection, Query1_3);
						if (!state) {
							sql_result = mysql_store_result(connection);
							cout << "Last successful Delivery ID of crashed truck : ";
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
								cout << sql_row[0] << ' ';
							mysql_free_result(sql_result);
							cout << endl;
							cout << endl;
						}
						continue;
					}
					case 0:
					{
						one_flag = 1;
						break;
					}
					
					}
					if (one_flag) break;
				}
				continue;
			}
			// query 2	
			case 2:
			{
				int s_year;
				cout << "\n\n---- TYPE ケ ----\n\n";
				cout << "** Find the customer who has shipped the most packages in certain year. **\n";
				cout << " Which Year? : ";
				cin >> s_year;

				char Query2_1[MAXLINE] = "select customer_id from delivery where year(delivery_date) = ";
				char Query2_2[MAXLINE] = " group by customer_id having count(*) = (select count(*) as count from delivery where year(delivery_date) = ";
				char Query2_3[MAXLINE] = " group by customer_id order by count desc limit 1)";
				char Query2[MAXLINE];
				sprintf(Query2, "%s%d%s%d%s", Query2_1, s_year, Query2_2, s_year, Query2_3);
				state = mysql_query(connection, Query2);
				if (!state) {
					sql_result = mysql_store_result(connection);
					cout << "The customer ID who has shipped the most packages in " << s_year << " : ";
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						cout << sql_row[0] << ' ';
					mysql_free_result(sql_result);
					cout << endl;
					cout << endl;
				}
				continue;
			}
			// query 3
			case 3:
			{
				int m_year;
				cout << "\n\n---- TYPE ゲ ----\n\n";
				cout << "** Find the customer who has spent the most money on shipping in the past year. **\n";
				cout << " Which Year? : ";
				cin >> m_year;

				char Query3_1[MAXLINE] = "select customer_id from statement where extract(year from billed_date) = ";
				char Query3_2[MAXLINE] = " group by customer_id order by sum(paid) desc limit 1";
				char Query3[MAXLINE];
				sprintf(Query3, "%s%d%s", Query3_1, m_year, Query3_2);

				state = mysql_query(connection, Query3);
				if (!state) {
					sql_result = mysql_store_result(connection);
					cout << "The customer ID who has spent the most money on shipping in " << m_year << " : ";
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						cout << sql_row[0] << ' ';
					mysql_free_result(sql_result);
					cout << endl;
					cout << endl;
				}
				continue;
			}
			// query 4
			case 4:
			{
				cout << "\n\n---- TYPE コ ----\n\n";
				cout << "** Find the packages that were not delivered within the promised time. **";

				char Query4[MAXLINE] = "select package_id from delivery where delivery_date > anticipate_date or (delivery_date is null and anticipate_date < '2023-06-07')";
				state = mysql_query(connection, Query4);
				if (!state) {
					sql_result = mysql_store_result(connection);
					cout << "Each package ID that was not delivered within the promised time : ";
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						cout << sql_row[0] << ' ';
					mysql_free_result(sql_result);
					cout << endl;
					cout << endl;
				}
				continue;
			}
			// query 5
			case 5:
			{
				int year, month, cus_id;
				cout << "Input customer_id : ";
				cin >> cus_id;
				cout << "Input year : ";
				cin >> year;
				cout << "Input month : ";
				cin >> month;
				cout << "\n\n---- TYPE ゴ ----\n\n";
				cout << "** Generate the bill for each customer for the past month. Consider creating several types of bills. **\n\n\n";
				
				char Query5_1_1[MAXLINE] = "select ci.customer_id, ci.address, st.not_paid from customer_information ci natural join statement st where extract(year from st.billed_date) = ";
				char Query5_1_2[MAXLINE] = " and extract(month from st.billed_date) = ";
				char Query5_1_3[MAXLINE] = " and ci.customer_id = ";
				char Query5_1[MAXLINE];
				sprintf(Query5_1, "%s%d%s%d%s%d", Query5_1_1, year, Query5_1_2, month, Query5_1_3, cus_id);

				
				state = mysql_query(connection, Query5_1);
				if (!state) {
					sql_result = mysql_store_result(connection);
					cout << "First, Simple bill\n\n";
					cout << "customer_id address not_paid\n";
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						cout << sql_row[0] << ' ';
						cout << sql_row[1] << ' ';
						cout << sql_row[2] << endl;
					}
					mysql_free_result(sql_result);
					cout << endl;
					cout << endl;
				}

				char Query5_2_1[MAXLINE] = "select s.package_id, (case when p.contents_type = 'Overseas' then '100000' when p.contents_type = 'Danger' then '50000' when p.contents_type = 'Overseas_Danger' then '200000' when p.contents_type = 'None' then '0' else '0' end) as contents_type_fee, (case when p.wrapped_type = 'big' then '50000' when p.wrapped_type = 'normal' then '30000' when p.wrapped_type = 'small' then '10000' else '0' end) as wrapped_type_fee from service s join delivery d on s.delivery_id = d.delivery_id and s.package_id = d.package_id join vehicle v on d.vehicle_id = v.vehicle_id join package p on s.customer_id = p.customer_id and s.package_id = p.package_id join statement st on s.statement_id = st.statement_id and s.customer_id = st.customer_id where s.customer_id = ";
				char Query5_2_2[MAXLINE] = " and extract(year from st.billed_date) = ";
				char Query5_2_3[MAXLINE] = " and extract(month from st.billed_date) = ";
				char Query5_2[MAXLINE]; 
				sprintf(Query5_2, "%s%d%s%d%s%d", Query5_2_1, cus_id, Query5_2_2, year, Query5_2_3, month);


				state = mysql_query(connection, Query5_2);
				if (!state) {
					sql_result = mysql_store_result(connection);
					cout << "Second, Charge bill\n\n";
					cout << "package_id contents_type_fee wrapped_type_fee\n";
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						cout << sql_row[0] << ' ';
						cout << sql_row[1] << ' ';
						cout << sql_row[2] << endl;
					}
					mysql_free_result(sql_result);
					cout << endl;
					cout << endl;
				}

				char Query5_3_1[MAXLINE] = "select v.vehicle_id, d.package_id, (case when mod(v.vehicle_id, 2) = 0 then 50000 else 10000 end) as vehicle_fee from service s join delivery d on s.delivery_id = d.delivery_id and s.package_id = d.package_id join vehicle v on d.vehicle_id = v.vehicle_id join statement st on s.statement_id = st.statement_id and s.customer_id = st.customer_id where s.customer_id = ";
				char Query5_3_2[MAXLINE] = " and extract(year from st.billed_date) = ";
				char Query5_3_3[MAXLINE] = " and extract(month from st.billed_date) = ";
				char Query5_3[MAXLINE];
				sprintf(Query5_3, "%s%d%s%d%s%d", Query5_3_1, cus_id, Query5_3_2, year, Query5_3_3, month);

				state = mysql_query(connection, Query5_3);
				if (!state) {
					sql_result = mysql_store_result(connection);
					cout << "Third, Itemized shipment bill\n";
					cout << "vehicle_id package_id vehicle_fee\n";
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						cout << sql_row[0] << ' ';
						cout << sql_row[1] << ' ';
						cout << sql_row[2] << endl;
					}
						
					mysql_free_result(sql_result);
					cout << endl;
					cout << endl;
				}
				continue;
			}
			case 0:
			{
				flag = 1;
				break;
			}
			default:
				cout << "type number is wrong. retry!\n";
			}
			if (flag) break;
		}
		mysql_close(connection);
	}

	// drop table
	rfp.open("20211606_drop.txt");
	if (rfp.fail()) {
		cout << "file open error\n";
		return 0;
	}
	string str;
	while (getline(rfp, str)) {
		query = str.c_str();
		mysql_query(connection, query);
	}
	rfp.close();

	return 0;
}

	
