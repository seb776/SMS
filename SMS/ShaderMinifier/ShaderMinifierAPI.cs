using Antlr4.Runtime;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ShaderMinifier
{
    //public class OriginalCodeVisitor : GLSLBaseVisitor
    //{

    //}

    public class ShaderMinifierAPI
    {
        public static string Minify(string shaderCode)
        {
            AntlrInputStream input = new AntlrInputStream(shaderCode);
            GLSLLexer lexer = new GLSLLexer(input);
            CommonTokenStream tokens = new CommonTokenStream(lexer);
            GLSLParser parser = new GLSLParser(tokens);
            //parser.Ac
            // need to add visitors so 
            //https://www.antlr2.org/doc/trees.html
            //https://stackoverflow.com/questions/29929062/antlr4-ast-vistor-and-node-return-type-should-i-have-more-than-one-visitor
            // https://stackoverflow.com/questions/23092081/antlr4-visitor-pattern-on-simple-arithmetic-example
            //var unit = parser.translation_unit();

            //unit.external_declaration_list().external_declaration_list().GetChild(0)
            return shaderCode;// unit.ToStringTree();
        }
    }
}
