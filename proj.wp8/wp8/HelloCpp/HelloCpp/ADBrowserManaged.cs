using System;
using Microsoft.Phone.Tasks;
namespace PhoneDirect3DXamlAppComponent
{
    public partial class ADBrowserManaged
    {

        public ADBrowserManaged()
        {
            BrowserHelper.setOpenUrlDelegate(openUrl);
        }

        void openUrl(string url)
        {
            WebBrowserTask webBrowserTask = new WebBrowserTask();
            webBrowserTask.Uri = new Uri(url, UriKind.Absolute);
            webBrowserTask.Show();
        }

        public static void init()
        {
            _obj = new ADBrowserManaged();
        }
        static ADBrowserManaged _obj = null;
    }
}