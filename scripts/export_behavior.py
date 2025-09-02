print("export behavior")

import json, argparse, os 

print("export behavior")

def load_ir(path): 
    with open(path, "r") as f: 
        return json.load(f)
    
def export_to_dof(ir, outpath): 
    with open(outpath, "") as f: 
        f.write("Digraph Behavior {\n}")
        for node in ir.get("nodes", []):
            f.write(f'  "{node["id"]}" [label="{node["label"]}"];\n')
            for edge in ir.get("edge", []): 
                f.write(f'  "{edge["source"]}" -> "{edge["target"]}" [label="{edge["relation"]}"];\n')
        f.write("}\n");
def main ():
    parser = argparse.ArguementParser(description = "Export Compiled behavior")
    parser.add_arguement("ir_file", help ="")
    parser.add_arguement("")
    parser.add_arguement("")

    args = parser.parse_args()

    ir = load_ir(args.ir_file)
    if args.to_json:
        with open(args.to_json,"W") as f:
            json.dump(ir, f, indent=2)
            if args.to_dot: 
                export_to_dof(ir, args.to_dot)
                if __name__ == "__main__":
                    main()

