using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

// Written By Matthew Beck
// January, '15


//		
//		
//		  ____         _____      
//		 / ___| ___   |_   _|__   
//		| |  _ / _ \    | |/ _ \  
//		| |_| | (_) |   | | (_) | 
//		 \____|\___/    |_|\___/  
//		                          
//      http://bigtext.org/	
//		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace BigTexCommenter
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            List<Pattern> patternList = new List<Pattern>();

            
            List<string> PrecedingLines;
            string PrecedingColumns;
            List<string> SuccessiveLines;
            string Footer;
             for (int i = 0; i < 3; i++)
			{
                 PrecedingLines = new List<string>();
                 SuccessiveLines = new List<string>();

                 PrecedingColumns = "//";
                 for (int j = 0; j < i; j++)
                     PrecedingColumns += "\t";
                 for (int k = 0; k < i; k++)
			         PrecedingLines.Add(PrecedingColumns);
                 for (int j = 0; j < i; j++)
                    SuccessiveLines.Add(PrecedingColumns);
                 
                 Footer = "//////////////////////////////////////////////////////////////////////////////////////////////////////////";
                 for (int l = 0; l < i; l++)
                     Footer += "//////";
			     SuccessiveLines.Add(Footer);

                 patternList.Add(
                     new Pattern(
                         this,
                         i, 
                         PrecedingLines.ToArray(), 
                         PrecedingColumns, 
                         SuccessiveLines.ToArray()));
			}


            InitializeComponent();


        }




    }

    

    public class Pattern
    {
        private Form ParentForm;
        private RichTextBox TB;

        public int ID;
        public string[] PrecedingLines;
        public string PrecedingColumns;
        public string[] SuccessiveLines;

        public Pattern(Form _parentForm, int _ID, string[] _PrecedingLines, string _PrecedingColumns, string[] _SuccessiveLines)
        {
            ParentForm = _parentForm;
            ID = _ID;
            PrecedingLines = _PrecedingLines;
            PrecedingColumns = _PrecedingColumns;
            SuccessiveLines = _SuccessiveLines;

            TB = new System.Windows.Forms.RichTextBox();
            TB.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.TB.Font = new System.Drawing.Font("Consolas", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TB.Location = new System.Drawing.Point(12, 150 + (ID - 1) * 125);
            this.TB.Name = "TB" + ID;
            this.TB.Size = new System.Drawing.Size(350, 121);
            this.TB.TabIndex = 0;
            this.TB.Text = "";
            this.TB.TextChanged += new System.EventHandler(TextChanged);
            ParentForm.Controls.Add(this.TB);
        }

        private void TextChanged(object sender, EventArgs e)
        {
            Draw(((RichTextBox)sender).Lines);
        }

        public void Draw(string[] pastedLines)
        {
            foreach (string line in PrecedingLines)
                Console.WriteLine(line);
            foreach (string line in pastedLines)
                Console.WriteLine(PrecedingColumns + line);
            foreach (string line in SuccessiveLines)
                Console.WriteLine(line);
        }

    }


}
