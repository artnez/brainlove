#! /usr/bin/env julia

if length(ARGS) == 0
    write(STDERR, "Usage: brainlove SOURCE_FILE\n")
    exit(1)
end

const MEMSIZE = 3000
const SYMBOLS = ['>' '<' '+' '-' '.' ',' '[' ']']

type State
    ops::Array
    len::Integer
    idx::Integer
    skd::Integer
    mem::Array
    ptr::Integer
    State(ops, mem) = new(ops, length(ops), 0, 0, mem, 1)
end

function run(state::State)
    while state.idx < state.len
        state.idx += 1

        tok = state.ops[state.idx]
        if state.skd > 0
            if tok == "[" state.skd += 1 end
            if tok == "]" state.skd -= 1 end
            continue
        end

        if tok == "["
            if state.mem[state.ptr] == 0
                state.skd += 1
                continue
            end
            pos = state.idx
            while state.mem[state.ptr] != 0
                state.idx = pos
                run(state)
            end
            continue
        end

        if     tok == ">" state.ptr += 1
        elseif tok == "<" state.ptr -= 1
        elseif tok == "+" state.mem[state.ptr] += 1
        elseif tok == "-" state.mem[state.ptr] -= 1
        elseif tok == "." print(char(state.mem[state.ptr]))
        elseif tok == "," state.mem[state.ptr] = read(STDIN, Int8)
        elseif tok == "]" return
        end
    end
    println()
end

mem = zeros(Int8, MEMSIZE)
ops = split(filter(x -> in(x, SYMBOLS), open(readall, ARGS[1])), "")
@time run(State(ops, mem))
