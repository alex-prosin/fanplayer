using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace csharpplayer
{
    public partial class PlayerForm : Form
    {
        [DllImport("fanplayer.dll", EntryPoint = "player_open", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr player_open([MarshalAs(UnmanagedType.LPStr)] string file, IntPtr hwnd, IntPtr configs);

        [DllImport("fanplayer.dll", EntryPoint = "player_close", CallingConvention = CallingConvention.Cdecl)]
        static extern void player_close(IntPtr player);

        [DllImport("fanplayer.dll", EntryPoint = "player_play", CallingConvention = CallingConvention.Cdecl)]
        static extern void player_play(IntPtr player);

        [DllImport("fanplayer.dll", EntryPoint = "player_pause", CallingConvention = CallingConvention.Cdecl)]
        static extern void player_pause(IntPtr player);

        [DllImport("fanplayer.dll", EntryPoint = "player_seek", CallingConvention = CallingConvention.Cdecl)]
        static extern void player_seek(IntPtr player, long ms);

        [DllImport("fanplayer.dll", EntryPoint = "player_setrect", CallingConvention = CallingConvention.Cdecl)]
        static extern void player_setrect(IntPtr player, int type, int x, int y, int w, int h);

        [DllImport("fanplayer.dll", EntryPoint = "player_snapshot", CallingConvention = CallingConvention.Cdecl)]
        static extern int player_snapshot(IntPtr player, [MarshalAs(UnmanagedType.LPStr)] string file, int w, int h, int wait);

        [DllImport("fanplayer.dll", EntryPoint = "player_setparam", CallingConvention = CallingConvention.Cdecl)]
        static extern void player_setparam(IntPtr player, uint id, IntPtr param);

        [DllImport("fanplayer.dll", EntryPoint = "player_getparam", CallingConvention = CallingConvention.Cdecl)]
        static extern void player_getparam(IntPtr player, uint id, IntPtr param);

        private IntPtr mPlayer;

        public PlayerForm()
        {
            InitializeComponent();
        }

        private void PlayerForm_Load(object sender, EventArgs e)
        {
            mPlayer = player_open("c:\\test.mp4", this.Handle, (IntPtr)0);
            player_play(mPlayer);
        }

        private void PlayerForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            player_close(mPlayer);
        }
    }
}
