//
// MainPage.xaml.cpp
// MainPage 类的实现。
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace CppTestApp;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace MediaCodecRT;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;
using namespace concurrency;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

MainPage::MainPage()
{
	InitializeComponent();
}


void CppTestApp::MainPage::Grid_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void CppTestApp::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	DllLoader ^ loader = ref new DllLoader();
	bool res = loader->LoadAll();
	//VideoFrames^ vf = ref new VideoFrames();
	FileOpenPicker^ filePicker = ref new FileOpenPicker();
	filePicker->ViewMode = PickerViewMode::Thumbnail;
	filePicker->SuggestedStartLocation = PickerLocationId::Desktop;
	filePicker->FileTypeFilter->Append("*");

	create_task(filePicker->PickSingleFileAsync()).then([this](StorageFile^ file)
	{
		if (file != nullptr)
		{
			create_task(file->OpenAsync(FileAccessMode::Read)).then([this, file](task<IRandomAccessStream^> stream)
			{
				try
				{
					IRandomAccessStream^ readStream = stream.get();
					auto obj = VideoFrames::CreatVideoFrames(readStream,1080,720);
					obj->InitAutoIncrementFrameJpegBuffer();
					
					while (true)
					{
						InMemoryRandomAccessStream^ bbb = obj->GetAutoIncrementFrameJpegBuffer();
						if (bbb == nullptr || bbb->Size <= 0)
						{
							break;
						}
					}

					//obj->Close();

					delete(obj);
				}
				catch (COMException^ ex)
				{

				}
			});
		}
	});
}
