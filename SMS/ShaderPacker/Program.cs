using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ShaderPacker
{
    class Program
    {
        static void TransformFile(bool first, string outputHPath, string outputCPath, string path)
        {
            string name = System.IO.Path.GetFileNameWithoutExtension(path);
            string folderPath = System.IO.Path.GetDirectoryName(path);
            var content = File.ReadAllText(path);

            var headerOut = string.Format((first ? "#pragma once\n" : "") + (first ? "" : "\n") + "extern \"C\" const char {0}[];", name);

            File.AppendAllText(outputHPath, headerOut);

            string fixedContent = content.Replace("\r\n", "");

            var cOutput = string.Format((first ? "" : "\n") + "extern \"C\" const char {0}[{1}] = \"{2}\";", name, fixedContent.Count() + 1, fixedContent);

            File.AppendAllText(outputCPath, cOutput);

        }

        public static void MinifyTest(string inputFile, string outputFile)
        {
            var minShader = ShaderMinifier.ShaderMinifierAPI.Minify(File.ReadAllText(inputFile));

            File.WriteAllText(outputFile, minShader);
        }

        static void Main(string[] args)
        {
            /*
            var filesmin = new string[]
            {
                @"E:\z0rg\Projects\SMS\SMS\SMS\Shaders\DebugMathFunction.glsl",
                @"E:\z0rg\Projects\SMS\SMS\SMS\Shaders\Loading.glsl",
                @"E:\z0rg\Projects\SMS\SMS\SMS\Shaders\Pharma.glsl",
                @"E:\z0rg\Projects\SMS\SMS\SMS\Shaders\RetroReflect.glsl",
                @"E:\z0rg\Projects\SMS\SMS\SMS\Shaders\Shader2.glsl"
            };

            foreach(var file in filesmin)
            {
                MinifyTest(file, file + ".min");

            }
            return;
            */

            Console.WriteLine("Packing shaders...");
            if (args.Count() < 3)
            {
                throw new Exception("You have to provide three parameters, the output path and the file name, then the input folder");
            }
            string outputH = System.IO.Path.Combine(args[0], args[1] + ".h");
            string outputCPP = System.IO.Path.Combine(args[0], args[1] + ".cpp");

            try
            {

                File.Delete(outputH);
                File.Delete(outputCPP);
            }
            catch (Exception)
            {

            }

            Console.WriteLine(args[2]);
            var files = Directory.GetFiles(args[2]);
            bool first = true;
            foreach (var file in files)
            {

                var ext = System.IO.Path.GetExtension(file).ToLower();
                if (ext == ".glsl")
                {
                    TransformFile(first, outputH, outputCPP, file);
                    first = false;
                }
            }
        }
    }
}
