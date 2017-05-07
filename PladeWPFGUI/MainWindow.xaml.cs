using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace PladeWPFGUI
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("PladeParser.dll", CharSet = CharSet.Unicode, SetLastError = true, EntryPoint = "fnPladeParser", CallingConvention = CallingConvention.Cdecl)]
        public static extern int FnPladeParser([In]string FileName);

        [DllImport("PladeParser.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int OpenConsole();


        public MainWindow()
        {
            InitializeComponent();
            OpenConsole();
            var p = FnPladeParser("中文.cpp");
            
        }
    }
}
