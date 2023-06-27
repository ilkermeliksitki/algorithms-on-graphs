def print_adj_list(adj_list):
    if adj_list == 'adj_list':
        r = gdb.parse_and_eval("out_going_edge_count")       
    else:
        adj_list = 'reversed_adj_list'
        r = gdb.parse_and_eval("incoming_edge_count")       

    for i in range(gdb.parse_and_eval("n")):
        gdb.write(f'vertex ' + str(i) + '=>')
        for j in range(r[i]):
            # If `adj_list` is the name of your variable in C/C++ code:
            e = f"{adj_list}[{i}][{j}]"
            element = gdb.parse_and_eval(e)
            gdb.write(' ' + str(element))
        gdb.write('\n')

gdb.execute("break main")
gdb.execute("break find_sccs")
gdb.execute("break dfs")
gdb.execute("break reverse_adj_list")
gdb.execute("run < cases/03")

