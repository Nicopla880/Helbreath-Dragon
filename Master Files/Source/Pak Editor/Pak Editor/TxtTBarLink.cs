using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
namespace Pak_Editor
{
    class TxtTBarLink
    {
        private TextBox txtBox;
        private TrackBar trackBar;
        private int lowerBound;
        private int upperBound;
        private string setMethod;
        private static List<TxtTBarLink> links;

        public TxtTBarLink()
        {
        }

        private TxtTBarLink(TextBox _t, TrackBar _tb, string _f)
        {
            txtBox = _t;
            trackBar = _tb;
            setMethod = _f;
            lowerBound = 0;
            upperBound = 0;
        }

        public static void SetupLink(TextBox _t, TrackBar _tb, string _set)
        {
            if (links == null) links = new List<TxtTBarLink>();
            links.Add(new TxtTBarLink(_t, _tb, _set));
        }

        public void Clear()
        {
            links.Clear();
        }

        public static void SetValue(object o, int value, int lowerb = int.MaxValue, int upperb = int.MaxValue)
        {
            TxtTBarLink TTData = GetTTLink(o);
            if (TTData == null) return;
            if (lowerb != int.MaxValue && upperb != int.MaxValue)
            {
                TTData.lowerBound = lowerb;
                TTData.upperBound = upperb;
                TTData.trackBar.SetRange(lowerb, upperb);
            }
            TTData.txtBox.Text = value.ToString();
            if (value > TTData.trackBar.Maximum) TTData.trackBar.Maximum = value;
            if (value < TTData.trackBar.Minimum) TTData.trackBar.Minimum = value;
            TTData.trackBar.Value = value;
        }

        public static string GetSetMethod(object o)
        {
            TxtTBarLink TTData = GetTTLink(o);
            if (TTData == null) return "";

            return TTData.setMethod;
        }

        private static TxtTBarLink GetTTLink(object o)
        {
            if (o.GetType().Name == "TextBox")
            {
                foreach (TxtTBarLink TTData in links)
                if (TTData.txtBox == (TextBox)o)
                {
                    return TTData;
                }
            }
            else if (o.GetType().Name == "TrackBar")
            {
                foreach (TxtTBarLink TTData in links)
                if (TTData.trackBar == (TrackBar)o)
                {
                    return TTData;
                }
            }

            return null;
        }

        public static void GetBounds(object o, out int lower, out int upper)
        {
            lower = 0;
            upper = 0;
            TxtTBarLink TTData = GetTTLink(o);
            if (TTData == null) return;

            lower = TTData.lowerBound;
            upper = TTData.upperBound;
        }
    }
}
