import pandas as pd
import cx_Oracle
x = pd.read_excel("Your-Excel-Path.xlsx")
y = list(x.itertuples(index=False, name=None))
try:
    conn = cx_Oracle.connect('username', 'password', 'localhost:1000/dbname')

except Exception as e:
    print('Exception occured', e)

else:
    try:
        for i in y:
            cur = conn.cursor()
            query = ("UPDATE <TABLENAME> SET <VAL1> = '" + str(i[1]) + "' WHERE <VAL2> = '" + str(i[0]) + "'")
            result = cur.execute(query)
            conn.commit()
    except Exception as e:
        print('Exception occured', e)

finally:
    conn.close()