import psycopg2
import numpy as np
import os
import argparse


def get_parameters():
    """ Parse command line parameters """
    parser = argparse.ArgumentParser()

    parser.add_argument("-p", "--password", default="postgres", help="Password for psql user")
    parser.add_argument("-u", "--user", default=os.environ['USER'], help="Psql user name")
    parser.add_argument("-d", "--db_name", default='fs_db', help="Psql databse name")
    return parser.parse_args()


def get_table_names(conn):
    cur = conn.cursor()
    get_tables_cmd = "select tablename from pg_tables where schemaname = 'public';"
    cur.execute(get_tables_cmd)
    table_names = np.array(cur.fetchall()).flatten()
    cur.close()
    return table_names


def drop_tables(conn, table_names):
    cur = conn.cursor()
    for tname in table_names:
        drop_table_cmd = "drop table if exists {};".format(tname)
        print(drop_table_cmd)
        cur.execute(drop_table_cmd)
    conn.commit()
    cur.close()


if __name__ == "__main__":
    args = get_parameters()
    print("dbname={} user={} password={}".format(args.db_name, args.user, args.password))
    conn = psycopg2.connect("dbname={} user={} password={}".format(args.db_name, args.user, args.password))
    cur = conn.cursor()
    table_names = get_table_names(conn)
    drop_tables(conn, table_names)
    conn.close()
