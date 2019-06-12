#pragma once
/*
SuaKIT runtime API 2.2.4.11
Copyright (c) 2018 SuaLab, All right reserved.
*/
/**
* @file SuaKIT.h
*/

#ifndef __GNUC__
#ifdef SUAKIT_CORELIB_DLL
#define SUAKIT_API __declspec(dllexport)
#else
#define SUAKIT_API __declspec(dllimport)
#endif
#else
#define SUAKIT_API __attribute__ ((visibility ("default")))
#define __stdcall
#endif

/*
GetClassName => GetClassLabelName
GetMessage  =>  GetTrainMessage
*/
#ifndef GetClassName
#define GetClassName GetClassLabelName
#endif
#ifndef GetMessage
#define GetMessage GetTrainMessage
#endif

typedef int SuaKIT_Int32;
typedef unsigned long long SuaKIT_UInt64;
typedef long long SuaKIT_Int64;
typedef unsigned char uchar;

#ifdef SWIG
%include <typemaps.i>

%apply unsigned char INPUT[]{ uchar *dataPtr }
%apply unsigned char INPUT[]{ uchar *buffer }

%apply float& INOUT{ float& minLoss };
%apply int& INOUT{ SuaKIT_Int32& minIteration };
%apply float& INOUT{ float& recentLoss };
%apply int& INOUT{ SuaKIT_Int32& recentIteration };
%apply long long& INOUT{ SuaKIT_Int32& width };
%apply long long& INOUT{ SuaKIT_Int32& height };
%apply unsigned long long& INOUT{ SuaKIT_UInt64& gpuFreeMemSize };
#endif

namespace SuaKIT {
	namespace API {
		/** @brief Indicates the status of execution result of SuaKIT API */
		enum Status {
			SUCCESS = (0), /**< Success */
			ERR_INPUT = (1), /**< Input error */
			ERR_SYSTEM = (2), /**< System error */
			ERR_LOGIC = (3), /**< Logic error */
			ERR_PROTECTION = (4), /**< Protection error(check hardware protection dongle key) */
			ERR_INITIALIZE = (5), /**< Initialization error */
			ERR_TIMEOUT = (6), /**< Timeout */
			ERR_NOTIMPL = (7), /**< Not implemented */
			ERR_FATAL = (0xFFFE), /**< Fatal system error */
			ERR_UNKNOWN = (0xFFFF) /**< Unknown error */
		};

		/** @brief Indicates the Device type (CPU, GPU) */
		enum DeviceKind {
			DeviceKind_NON = (0), /**< NONE */
			DeviceKind_CPU = (1), /**< CPU */
			DeviceKind_GPU = (2)  /**< GPU */
		};

		/** @brief Indicates the depth of the image. Depth is the "precision" of each pixel.*/
		enum DepthType {
			_8U = (1), /**< unsigned 8bit (Commonly used)*/
			_16U = (2) /**< unsigned 16bit */
		};

		/** @brief Indicates the input type for the SuaKIT network.*/
		enum InputDataType {
			NON = (0), /**< NONE */
			SINGLE = (1), /**< Single Image - Use single image (Commonly used)*/
			COMPARISON = (2), /**< Image Comparison - Compare two images*/
			MULTIMG = (3) /**< Multiple Image - N images become a unit*/
		};
		
		/** @brief Indicates the model type for the SuaKIT network model.*/
		enum ModelType {
			UNKNOWN_MODEL_TYPE = (0), /**< None */
			CLASSIFICATION = (1), /**< Classification */
			SEGMENTATION = (2), /**< Segmentation - default(Sensitive)*/
			DETECTION = (3), /**< Detection */
			ONECLASS_SEGMENTATION = (4), /**< OneClass segmentation */
			DETECTION_DOLPHIN = (5), /**< Detection - Dolphin */
			DETECTION_DRAGON = (6), /**< Detection - Dragon */
			POST_TRAIN = (7),	/**< PostTrain */
			SEGMENTATION_SENSITIVE = (8), /**< Segmentation - Sensitive */
			SEGMENTATION_CONTEXTUAL = (9), /**< Segmentation - Contextual */
		};

		enum ModelFormatCvt {
			CNTKtoSuaKIT = (0),
		};

		/** @brief Structure containing rectangular information*/
		struct SUAKIT_API Rect {
			Rect(SuaKIT_UInt64 _x, SuaKIT_UInt64 _y, SuaKIT_UInt64 _width, SuaKIT_UInt64 _height, SuaKIT_UInt64 classNum = 0) : x(_x), y(_y), width(_width), height(_height), classNumber(classNum) {};
			Rect() :x(0), y(0), width(0), height(0), classNumber(0) {};
			bool IsEmpty() { return x == 0 && y == 0 && width == 0 && height == 0; };
			SuaKIT_UInt64 x, y, width, height, classNumber;
		};

		/** @brief Structure containing rectangular information for detection.*/
		struct SUAKIT_API DetectionRect {
			SuaKIT_UInt64 x, y, width, height;
			SuaKIT_UInt64 classNumber;
			float prob;
			float uncertainty;
		};

		/**
		* @brief Class regarding image data.
		* @details Used to send and receive image data when using the SuaKIT Runtime API. Depending on constructor options, resources are managed through reference counting.
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API ImageData {
		public:

			/** @brief As the default constructor that is used when receiving output image data, it is used to create an image data variable that will receive the output.*/
			__stdcall ImageData();
			__stdcall ~ImageData();

#ifndef SWIG
			/**
			* @brief Takes in an image pointer to create an ImageData instance.
			* @param dataPtr Input for the starting memory address of the image. The image data referenced by this memory address must be continuous.
			* @param step Input for the memory size for one line of the image. Normally, it is a width * channel value.
			* @param width Input for the width of the image as an integer. The width value must be 96 or greater, and it is highly recommended to use a multiple of 4.
			* @param height Input for the height of the image as an integer. The height value must be 96 or greater.
			* @param channel Input for the number of channels for the image as an integer. Currently, only 1 channel images, 3 channel images in BGR format, or 4 channel images in BGRA format are supported.
			* @param copyOption The ImageData resource management policy is set through the copyOption parameter.
			#1 copyOption: When true, a memory copy operation is carried out by the constructor, and resources are managed by reference counting.
			#2 copyOption: When false, memory is not copied and the data is accessed through the buffer address inputted by the user. In this case, the user is responsible for memory resources.
			* @param roi Specifies the roi area of the image. For an empty Rect, which is the default value, the value is ignored.
			* @param depth Indicates the depth of the image. Depth is the "precision" of each pixel. _8U (unsigned 8bit) is commonly used.
			*/
			__stdcall ImageData(uchar* dataPtr,
				SuaKIT_UInt64 step,
				SuaKIT_UInt64 width,
				SuaKIT_UInt64 height,
				SuaKIT_UInt64 channel,
				bool copyOption,
				Rect roi = Rect(),
				DepthType depth = DepthType::_8U
			);
#endif
			/**
			* @brief Takes in an image pointer to create an ImageData instance.
			* @param dataPtr Input for the starting memory address of the image. The image data referenced by this memory address must be continuous.
			* @param step Input for the memory size for one line of the image. Normally, it is a width * channel value.
			* @param width Input for the width of the image as an integer. The width value must be 96 or greater, and it is highly recommended to use a multiple of 4.
			* @param height Input for the height of the image as an integer. The height value must be 96 or greater.
			* @param channel Input for the number of channels for the image as an integer. Currently, only 1 channel images, 3 channel images in BGR format, or 4 channel images in BGRA format are supported.
			* @param roi Specifies the roi area of the image. For an empty Rect, which is the default value, the value is ignored.
			* @param depth Indicates the depth of the image. Depth is the "precision" of each pixel. _8U (unsigned 8bit) is commonly used.
			*/
			__stdcall ImageData(uchar* dataPtr,
				SuaKIT_UInt64 step,
				SuaKIT_UInt64 width,
				SuaKIT_UInt64 height,
				SuaKIT_UInt64 channel,
				Rect roi = Rect(),
				DepthType depth = DepthType::_8U
			);

			/**
			* @brief Takes in an image file path to create an ImageData instance.
			* @param paths Input for the paths of image file. When using Image Comparison or MultiImage Type models, each image in an ImageData Separate by newline then pushed in this. In this case, you do not need to use the PushImage method.
			* @param roi Specifies the roi area of the image. For an empty Rect, which is the default value, the value is ignored.
			* @param channel Input for the number of channels for the image as an integer. Currently, only 1 channel images, 3 channel images in BGR format, or 4 channel images in BGRA format are supported.
			* @param depth Indicates the depth of the image. Depth is the "precision" of each pixel. _8U (unsigned 8bit) is commonly used.
			*/
			__stdcall ImageData(const wchar_t * paths, Rect roi = Rect(), SuaKIT_UInt64 channel = 0, DepthType depth = DepthType::_8U);

			/**
			* @brief Creates a blank image of the input size.
			* @param step Input for the memory size for one line of the image. Normally, it is a width * channel value.
			* @param width Input for the width of the image as an integer. The width value must be 96 or greater, and it is highly recommended to use a multiple of 4.
			* @param height Input for the height of the image as an integer. The height value must be 96 or greater.
			* @param channel Input for the number of channels for the image as an integer. Currently, only 1 channel images, 3 channel images in BGR format, or 4 channel images in BGRA format are supported.
			* @param depth Indicates the depth of the image. Depth is the "precision" of each pixel. _8U (unsigned 8bit) is commonly used.
			*/
			__stdcall ImageData(
				SuaKIT_UInt64 step,
				SuaKIT_UInt64 width,
				SuaKIT_UInt64 height,
				SuaKIT_UInt64 channel,
				DepthType depth = DepthType::_8U
			);

			/**
			* @brief Push image for MultiImage or Image Comparison. When using Image Comparison or MultiImage type models, use this method to push each image in an ImageData.
			The size values of the images that are pushed into an ImageData object must all be the same. Otherwise, the method fails.
			* @param dataPtr Pointer to the address containing the actual image pixel data, such as `data` member of `cv::Mat`.
			* @param step Input for the memory size for one line of the image. Normally, if width is a multiple of 4, it is a width * channel value.
			* @param width Input for the width of the image as an integer. The width value must be 96 or greater, and it is highly recommended to use a multiple of 4.
			* @param height Input for the height of the image as an integer. The height value must be 96 or greater.
			* @param channel Input for the number of channels for the image as an integer. Currently, only 1 channel images, 3 channel images in BGR format, or 4 channel images in BGRA format are supported.
			* @param copyOption The ImageData resource management policy is set through the copyOption parameter.
			#1 copyOption: When true, a memory copy operation is carried out by the constructor, and resources are managed by reference counting.
			#2 copyOption: When false, memory is not copied and the data is accessed through the buffer address inputted by the user. In this case, the user is responsible for memory resources.
			* @param depth Indicates the depth of the image. Depth is the "precision" of each pixel. _8U (unsigned 8bit) is commonly used.
			* @return Returns Status. The size values of the images that are pushed into an ImageData object must all be the same. Otherwise, the method fails.
			*/
			Status __stdcall PushImage(uchar* dataPtr,
				SuaKIT_UInt64 step,
				SuaKIT_UInt64 width,
				SuaKIT_UInt64 height,
				SuaKIT_UInt64 channel,
				bool copyOption,
				DepthType depth = DepthType::_8U
			);

			/**
			* @brief Copies the image data to the user buffer.
			* @param buffer The address of the user buffer.
			* @param bufferByteSize The size of the user buffer in bytes.
			* @param index Indicates the index of each image in the ImageData object (0-based) when using Image Comparison or MultiImage type models.
			*/
			Status __stdcall CopyToBuffer(uchar* buffer, SuaKIT_UInt64 bufferByteSize, SuaKIT_Int32 index = 0);

#ifndef SWIG
			/** @brief Undocumented method*/
			uchar* __stdcall GetDataPtr(SuaKIT_Int32 index = 0) const;
#endif
			/** @brief Set Image ROI*/
			Status __stdcall SetRoi(Rect roi);

			/** @brief Returns the step value for the image. */
			SuaKIT_UInt64 GetStep() const { return m_step; };
			/** @brief Returns the width value for the image.*/
			SuaKIT_UInt64 GetWidth() const { return m_width; };
			/** @brief Returns the height value for the image. */
			SuaKIT_UInt64 GetHeight() const { return m_height; };
			/** @brief Returns the channel value for the image. */
			SuaKIT_UInt64 GetChannel() const { return m_channel; };

			/** @brief Returns the depth value for the image. */
			DepthType GetDepth() const { return m_depth; };
			/** @brief Returns the roi value for the image. */
			Rect GetRoi() const { return m_roi; };
			/** @brief Returns the object’s status code. If the object was instantiated normally, will return SUCCESS.*/
			Status GetStatus() const { return m_status; };
			/** @brief Returns the object’s copy option. */
			bool GetCopyOption() const { return m_copyOption; };

			/** @brief Returns whether the given ImageData is empty.*/
			bool __stdcall Empty();

			/** @brief This method Returns the number of images pushed to an ImageData object.
			(If you use an Image Comparison or MultiImage type model, you will push two or more images into an ImageData object.)
			*/
			SuaKIT_Int32 __stdcall GetImageCount() const;

			__stdcall ImageData(const ImageData &src);
			ImageData& __stdcall operator=(const ImageData& rhs);
#ifndef SWIG
			/** @brief Undocumented method*/
			void*  GetInternal() const { return internal; };
#endif		
			/** @brief Undocumented method*/
			static ImageData CreateMaskImageFromJson(SuaKIT_UInt64 width, SuaKIT_UInt64 height, const wchar_t* json, Rect roi = Rect()); //Undocumented methods

			/** @brief Undocumented method*/
			void __stdcall Save(const char * paths, SuaKIT_Int32 idx = 0);
		private:
			bool  __stdcall memberValueCheck();

			SuaKIT_UInt64 m_step;
			SuaKIT_UInt64 m_width;
			SuaKIT_UInt64 m_height;
			SuaKIT_UInt64 m_channel;
			DepthType m_depth;
			Rect m_roi;
			void* internal;
			Status m_status;
			bool m_copyOption;
		};

		/**
		* @brief An array wrapper class for a type (integer, float, Rect, DeviceDescriptor, Size) that is trivially copyable. 
		* @details
		* @author ⓒSualab SuaKIT Team
		*/
		template<typename VAL_TYPE>
		class DataArray {
		public:
			/** @brief As the default constructor that is used when receiving multiple pieces of output data, it is used to create an array that will receive the output. */
			DataArray() : m_startp(0), m_capacity(0), m_byteSize(0) { }
			/**
			* @brief Creates an array with the number of elements corresponding to size.
			* @param size Input for the size of the array to be created.
			*/
			DataArray(SuaKIT_UInt64 size);
			~DataArray();
			/** @brief Get the item value(reference) at any location. */
			const VAL_TYPE& GetAt(SuaKIT_UInt64 idx) const;

			/** @brief Get the item value at any location. */
			VAL_TYPE GetValue(SuaKIT_UInt64 idx) const;

			/** @brief Returns the byte size of the Array. */
			SuaKIT_UInt64 GetByteSize() const;

			/** @brief Returns the total number of items within the array.*/
			SuaKIT_UInt64 GetLength() const;

			/**
			* @brief Sets the input item at the idx position of the array.
			* @param idx Index position where the image data is to be set.
			* @param item The item to be set.
			*/
			Status SetAt(SuaKIT_UInt64 idx, const VAL_TYPE& item);

#ifndef SWIG 
			/** @brief Undocumented method*/
			VAL_TYPE* GetDataPtr();
			/** @brief Undocumented method*/
			const VAL_TYPE* GetDataPtr() const;
#endif

			/** @brief Undocumented method*/
			void _resize(SuaKIT_UInt64 size);

		private:
			DataArray(const DataArray & src); //=delete;
			DataArray& operator=(const DataArray& rhs); //=delete;
			SuaKIT_Int64 m_capacity;
			SuaKIT_Int64 m_byteSize;
			void* m_startp;
			void* internal;
		};

		/**
		* @brief An array wrapper class for ImageData. Copy constructors and assignment operators are forbidden.
		* @details
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API ImageArray {
		public:
			/** @brief As the default constructor that is used when receiving multiple pieces of output image data, it is used to create an image data array that will receive the output. */
			__stdcall ImageArray();
			/**
			* @brief Creates an array for the ImageData corresponding to the inputted size value.
			* @param size Input for the size of the array to be created.
			*/
			__stdcall ImageArray(SuaKIT_UInt64 size);
			__stdcall ~ImageArray();

			/** @brief Gets the ImageData at position idx. */
			const ImageData& __stdcall GetAt(SuaKIT_UInt64 idx) const;
			/** @brief Returns the total number of ImageData objects within the array. */
			SuaKIT_UInt64 GetLength() const { return m_length; }

			/**
			* @brief Sets the input item at the idx position of the array.
			* @param idx Index position where the image data is to be set.
			* @param item The item to be set.
			*/
			Status __stdcall SetAt(SuaKIT_UInt64 idx, const ImageData& item);
#ifndef SWIG 

			/** @brief Undocumented method*/
			Status __stdcall SetAt(
				SuaKIT_UInt64 idx,
				uchar* dataPtr,
				SuaKIT_UInt64 step,
				SuaKIT_UInt64 width,
				SuaKIT_UInt64 height,
				SuaKIT_UInt64 channel,
				bool copyOption = true
			);

			/** @brief Undocumented method*/
			void __stdcall _resize(SuaKIT_UInt64 size);
#endif
		private:
			ImageArray(const ImageArray & src); //=delete;
			ImageArray& operator=(const ImageArray& rhs); //=delete;
			SuaKIT_Int64 m_capacity;
			SuaKIT_Int64 m_length;
			ImageData* m_startp;
			void* internal;
		};

		/**
		* @brief Two-dimensional array wrapper to hold DetectionRect. Copy constructors and assignment operators are forbidden.
		* @details
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API DetRectArrayOfArray
		{
		public:

			__stdcall DetRectArrayOfArray();
			__stdcall ~DetRectArrayOfArray();

			/** @brief Returns the number of DetRectArray Items in DetRectArrayOfArray.*/
			SuaKIT_UInt64 __stdcall GetLength() const;
			/** @brief Returns the number of DetectionRect Items in the DetRectArray at idx_first.*/
			SuaKIT_UInt64 __stdcall GetLength(SuaKIT_UInt64 idx_first) const;
			/** @brief Retrieves the Item at the [idx_first][idx_second] position. */
			const DetectionRect& __stdcall GetAt(SuaKIT_UInt64 idx_first, SuaKIT_UInt64 idx_second)	const;
			/** @brief Removes the Item at [idx_first][idx_second] position. */
			Status __stdcall Remove(SuaKIT_UInt64 idx_first, SuaKIT_UInt64 idx_second);

			/**
			* @brief Sets the length of the array of arrays.
			* @param len_first Length of the array of arrays.
			*/
			Status __stdcall SetLength(SuaKIT_UInt64 len_first);
			/**
			* @brief Sets the length of the array at the idx_first position.
			* @param idx_first Index of the array whose length is set.
			* @param len_second Length of the idx_first-th array.
			*/
			Status __stdcall SetLength(SuaKIT_UInt64 idx_first, SuaKIT_UInt64 len_second);

			/**
			* @brief Sets the input item at the idx position of the array. ([idx_first][idx_second] = item)
			* @param idx_first Index position where the image data is to be set.
			* @param idx_second Index position where the image data is to be set.
			* @param item The item to be set.
			*/
			Status __stdcall SetAt(SuaKIT_UInt64 idx_first, SuaKIT_UInt64 idx_second, const DetectionRect& item);

		private:
			DetRectArrayOfArray(const DetRectArrayOfArray& src);	//=delete;
			DetRectArrayOfArray& operator=(const DetRectArrayOfArray& rhs);	//=delete;

			void* internal;
		};

		/** @brief Structure for image size */
		struct Size { SuaKIT_UInt64 width, height; };

		class DeviceDescriptor;

		typedef DataArray<SuaKIT_Int64> Int64Array;
		typedef DataArray<SuaKIT_Int32> Int32Array;
		typedef DataArray<float> FloatArray;
		typedef DataArray<Rect> RectArray;
		typedef DataArray<DeviceDescriptor> DeviceDescriptorArray;
		typedef DataArray<Size> SizeArray;
		typedef DataArray<bool> BoolArray;
		
#if SWIG 
		%template(Int64Array)DataArray<SuaKIT_Int64>;
		%template(Int32Array)DataArray<int>;
		%template(FloatArray)DataArray<float>;
		%template(RectArray)DataArray<Rect>;
		%template(DeviceDescriptorArray)DataArray<DeviceDescriptor>;
		%template(SizeArray)DataArray<Size>;
		%template(BoolArray)DataArray<bool>;
#endif
		/**
		* @brief This is a configuration of augmentation in training time.
		* @details It is used as data augmentation setting parameter for Trainer classes.
		* @author ⓒSualab SuaKIT Team
		*/
		struct AugmentationConfig {
			AugmentationConfig() : m_lrFlip(false), m_udFlip(false), m_rotation90(false), m_contrast(false), m_brightness(false),
				m_zoomIn(false), m_sharpen(false), m_blur(false), m_colorwise(false), m_gradation(false),
				m_distortion(false), m_rotation(false), m_noise(false),
				m_brightnessMin(-3), m_brightnessMax(+3),
				m_sharpenKernelMin(0.1), m_sharpenKernelMax(0.8),
				m_gaussianBlurSigmaMin(0.4), m_gaussianBlurSigmaMax(1.5),
				m_constrastMin(0.7), m_constrastMax(1.3),
				m_noiseIntensity(0.1),
				m_occlusion(false)

			{};
			/**
			* @brief This is a constructor of the AugmentationConfig structure.
			* @param lrFlip Input parameter for left/right flip data augmentation option.
			* @param udFlip Input parameter for up/down flip data augmentation option.
			* @param rotation90 Input parameter for rotate 90 degree data augmentation option.
			* @param contrast Input parameter for contrast variation data augmentation option.
			* @param brightness Input parameter for brightness data augmentation option.
			* @param zoomIn Input parameter for zoomIn(cropping and resizing) data augmentation option.
			* @param sharpen Input parameter for sharpness data augmentation option.
			* @param blur Input parameter for blur(gaussian) data augmentation option.
			* @param colorwise Input parameter for colorwise data augmentation option.
			* @param gradation Input parameter for gradation data augmentation option.
			* @param distortion Input parameter for distortion data augmentation option.
			* @param rotation Input parameter for rotate random (0-45] degree data augmentation option.
			* @param noise Input parameter for noise data augmentation option.
			* @param brightnessMin Input parameter for brightness parameter min value. Recommended min value is -3
			* @param brightnessMax Input parameter for brightness parameter max value. Recommended min value is +3
			* @param sharpenKernelMin Input parameter for sharpen kernel parameter min value. Recommended min value is 0.1
			* @param sharpenKernelMax Input parameter for sharpen kernel parameter max value. Recommended min value is 0.8
			* @param gaussianBlurKernelMin Input parameter for gaussian kernel parameter min value. Recommended min value is 0.4
			* @param gaussianBlurKernelMax Input parameter for gaussian kernel parameter max value. Recommended min value is 1.5
			* @param contrastMin Input parameter for  noise contrast parameter max value. Recommended min value is 0.7
			* @param contrastMax Input parameter for  noise contrast parameter max value. Recommended min value is 1.3
			* @param noiseIntensity Input parameter for  noise augmentation parameter max value. Recommended min value is 0.1
			* @param occlusion Input parameter for occlusion data augmentation option.
			*/
			AugmentationConfig(
				bool lrFlip, bool udFlip, bool rotation90, bool contrast, bool brightness,
				bool zoomIn, bool sharpen, bool blur, bool colorwise, bool gradation,
				bool distortion, bool rotation, bool noise, int brightnessMin = -3, int brightnessMax = 3, double sharpenKernelMin = 0.1, double sharpenKernelMax = 0.8,
				double gaussianBlurKernelMin = 0.4, double gaussianBlurKernelMax = 1.5,
				double contrastMin = 0.7, double contrastMax = 1.3,
				double noiseIntensity = 0.1,
				bool occlusion = false)
				: m_lrFlip(lrFlip), m_udFlip(udFlip), m_rotation90(rotation90), m_contrast(contrast), m_brightness(brightness),
				m_zoomIn(zoomIn), m_sharpen(sharpen), m_blur(blur), m_colorwise(colorwise), m_gradation(gradation),
				m_distortion(distortion), m_rotation(rotation), m_noise(noise),
				m_brightnessMin(brightnessMin), m_brightnessMax(brightnessMax),
				m_sharpenKernelMin(sharpenKernelMin), m_sharpenKernelMax(sharpenKernelMax),
				m_gaussianBlurSigmaMin(gaussianBlurKernelMin), m_gaussianBlurSigmaMax(gaussianBlurKernelMax),
				m_constrastMin(contrastMin), m_constrastMax(contrastMax),
				m_noiseIntensity(noiseIntensity),
				m_occlusion(occlusion)

			{};
			bool m_lrFlip, m_udFlip, m_rotation90, m_contrast, m_brightness, m_zoomIn, m_sharpen, m_blur;
			bool m_colorwise, m_gradation, m_distortion, m_rotation, m_noise, m_reserve1, m_reserve2, m_reserve3;
			int m_brightnessMin, m_brightnessMax;
			double m_sharpenKernelMin, m_sharpenKernelMax;
			double m_gaussianBlurSigmaMin, m_gaussianBlurSigmaMax;
			double m_constrastMin, m_constrastMax;
			double m_noiseIntensity;
			bool m_occlusion;
		};

		/**
		* @brief Wrapper for string messages.
		* @details
		* @author ⓒSualab SuaKIT Team
		*/
		struct SUAKIT_API Message {

			/** @brief MessageType */
			enum MessageType {
				MSG_UNKNOWN = 0,
				MSG_GENERAL = 1,
				MSG_TRAIN_VALIDATIONLOSS = 2,
				MSG_TRAIN_MODELSAVE = 3,
				MSG_ERR = 4,
			};

			__stdcall Message();
			__stdcall Message(const wchar_t * data, SuaKIT_Int64 size, MessageType msgt);
			__stdcall ~Message();

			/**
			* @brief Returns the message string. The corresponding string buffer space is destroyed when this object is destroyed.
			* In the C ++ API, this method returns the c style wide character string type.
			*/
			const wchar_t* GetData() const { return m_data; }

			/**	 @brief Returns the length of a string containing null characters.*/
			SuaKIT_Int64 GetSize() const { return m_size; };

			/**	 @brief
			Returns the MessageType value of the current object.
			*/
			MessageType GetMsgType() const { return m_msgtype; }

			/**	 @brief
			If this object is a message of type MSG_TRAIN_MODELSAVE, the file path setting of the training result is returned.
			This function returns the value normally only when the return value of GetMsgType () is MSG_TRAIN_MODELSAVE.
			*/
			const wchar_t* __stdcall GetModelFilePath();
			/**	 @brief
			If this object is a message of MSG_TRAIN_VALIDATIONLOSS type, it outputs the current validation result information to the variable passed as a reference argument.
			This function works normally only when the return value of GetMsgType() is MSG_TRAIN_VALIDATIONLOSS.
			* @param minLoss The minimum value of the loss values ​​measured so far
			* @param minIteration Iteration value when the minimum value of the loss value is measured
			* @param recentLoss The most recently measured loss value
			* @param recentIteration Iteration value when measuring the most recently measured loss value
			*/
			void __stdcall GetValidationInfo(float& minLoss, SuaKIT_Int32& minIteration, float& recentLoss, SuaKIT_Int32& recentIteration);


			__stdcall Message(const Message& src);
			Message& __stdcall operator=(const Message& rhs);
		private:
			wchar_t * m_data;
			SuaKIT_Int64 m_size;

			wchar_t * m_dataEx;
			SuaKIT_Int64 m_sizeEx;
			MessageType m_msgtype;
		};

		/**
		* @brief Structure containing model information of trained model.
		* @details Each parameters are the same as TrainConfig parameters.
		* @author ⓒSualab SuaKIT Team
		*/
		struct SUAKIT_API TrainedModelInfo
		{
			AugmentationConfig augmentationConfig; /**< Augmentation parameters set at training.*/
			Int32Array dataRatio; /**< The dataRatio value set in the TrainConfig object at the training. See the dataRatio description in each TrainConfig for details.*/
			FloatArray epochForLossGraph; /**< Epoch values ​​for the Loss graph. The loss value corresponding to each epoch is stored in the same position in lossForLossGraph.*/
			FloatArray lossForLossGraph; /**< The loss value of the loss graph. The epoch values ​​for each loss are stored in the same location in epochForLossGraph.*/
			FloatArray anchors; /**< The anchors value set in the DetectionTrainConfig object at the training.This value is valid only on the Detection model.See the anchors description in DetectionTrainConfig for details.*/
			float minimumEpoch; /**< The minimumEpoch value set in the TrainConfig object at the training. See the minimumEpoch description in each TrainConfig for details.*/
			float savedModelEpoch;  /**< The training epoch value at the time the current model is saved.*/
			float minLoss; /**< The minimum loss value in the training period of the current model*/
			float endEpoch; /**< Epoch at the end of training.*/
			SuaKIT_UInt64 multiImgCount; /**< The multiImgCount value set in the TrainConfig object at the training. See the multiImgCount description in each TrainConfig for details.*/
			SuaKIT_UInt64 numberOfClasses; /**< The numberOfClasses value set in the TrainConfig object at the training. See the numberOfClasses description in each TrainConfig for details.*/
			SuaKIT_UInt64 imageHeight; /**< The imageHeight value set in the TrainConfig object at the training. In the case of segmentation, it indicates the patch size. See the imageHeight or patchSize description in each TrainConfig for details.*/
			SuaKIT_UInt64 imageWidth;  /**< The imageWidth value set in the TrainConfig object at the training. In the case of segmentation, it indicates the patch size. See the imageWidth or patchSize description in each TrainConfig for details.*/
			SuaKIT_UInt64 imageChannel; /**< The imageChannel value set in the TrainConfig object at the training. See the imageChannel description in each TrainConfig for details.*/
			SuaKIT_UInt64 validationRatio; /**< The validationRatio value set in the TrainConfig object at the training. See the validationRatio description in each TrainConfig for details.*/
			SuaKIT_UInt64 epochs; /**< The epochs value set in the TrainConfig object at the training. See the epochs description in each TrainConfig for details.*/
			SuaKIT_UInt64 modelCapacity;  /**< The modelCapacity value set in the TrainConfig object at the training. See the modelCapacity description in each TrainConfig for details.*/
			SuaKIT_UInt64 numModelSelection; /**< The numModelSelection value set in the TrainConfig object at the training. See the numModelSelection description in each TrainConfig for details.*/
			SuaKIT_UInt64 patchSize; /**< The patchSize value set in the SegmentationTrainConfig object at the training. See the patchSize description in SegmentationTrainConfig for details. This value is only valid for the segmentation model.*/
			SuaKIT_UInt64 strideRatio; /**< The strideRatio value set in the TrainConfig object at the training. See the strideRatio description in each TrainConfig for details.*/
			SuaKIT_UInt64 unlabeledRatio; /**< The unlabeledRatio value set in the SegmentationTrainConfig object at the training. See the unlabeledRatio description in SegmentationTrainConfig for details. This value is only valid for the segmentation model.*/
			SuaKIT_UInt64 numTrainImage; /**< Number of images used in the training set.*/
			SuaKIT_UInt64 numValidationImage; /**< Number of images used in the validation set.*/
			SuaKIT_Int64 patience; /**< The patience value set in the TrainConfig object at the training. See the patience description in each TrainConfig for details.*/
			static const uchar BUFF_SIZE = 32;
			char trainElapsedTime[BUFF_SIZE]; /**< A time value string that indicates how long the training has been. "dd, hh:mm:ss" format*/
			char modelSavedTime[BUFF_SIZE]; /**< Indicates the time the model finished training and saved as a file. "mm-dd-yyyy hh:mm" format*/
			char modelVersion[BUFF_SIZE]; /**< Indicates the SuaKIT version from which the current model was trained.*/
			ModelType modelType; /**<  Represents the ModelType of the current model. See ModelType description for details.*/
			InputDataType inputDataType; /**< The inputDataType value set in the TrainConfig object at the training. See the inputDataType description in each TrainConfig for details.*/
			DepthType imageDepth; /**< The imageDepth value set in the TrainConfig object at the training. See the imageDepth description in each TrainConfig for details.*/
		};

		/**
		* @brief Structure containing version of SuaKIT.
		* @author ⓒSualab SuaKIT Team
		*/
		struct SUAKIT_API SuaKIT_Version {
			SuaKIT_Int32 majorVersion;
			SuaKIT_Int32 minorVersion;
			SuaKIT_Int32 patchVersion;
			SuaKIT_Int32 buildNumber;
		};
        
		/**
		* @brief Class containing several unit functions.
		* @details All members are static method.
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API UnitFunction {
		public:
			/** @brief If the result status value of the SuaKIT API is not SUCCESS, it returns a message about the cause of the error. */
			static const char*  __stdcall GetErrorMessage();
			/** @brief Resizes the image.
			*
			*   This function resizes the internal image data of \p src and stores the result in \p dst.
			*   The channel and depth values are not changed by this function, whereas the step value may change.
			*   If there are more than one image in this ImageData, every image is resized.
			*   Below is an example code describing how to use this function.
			*
			*   @code
			*   ImageData src(L"path/to/256x256/image", Rect(), 1); // Load 256x256 1-channel image
			*   ImageData dst(128, 128, 512, 1);                    // An empty 128x512 1-channel image
			*   ResizeImage(src, dst);                              // Resizes src to 128x512
			*   @endcode
			*
			*   @note If roi was set on the \p src image data, the roi is also resized on the \p dst,
			*         and the resize ratio is proportional to the image resize ratio. For example,
			*         if an 128x128 image had roi set with (x=0, y=0, width=64, height=64) and the image is resized
			*         to 256x256, the roi of the resized image becomes (x=0, y=0, width=128, height=128).
			*
			*   @param src Original image data.
			*   @param dst Resized image data.
			*
			*   @author ⓒSualab SuaKIT Team
			*/
			static Status __stdcall ResizeImage(const ImageData& src, ImageData& dst);

			/**	 @brief Resize the image size.
			* @param src Original image data.
			* @param dst Resized image. When you input an empty imageData object, the resized image is saved to the object. If it is not a empty imageData object, the existing image data object is destroyed then replaced with the resized image.
			* @param width Size of resized Image width
			* @param height Size of resized Image height
			*/
			static Status __stdcall ResizeImage(const ImageData& src, ImageData& dst, SuaKIT_UInt64 width, SuaKIT_UInt64 height);

			/**	 @brief  Crop the image.
			* @param src Original image data.
			* @param dst Cropped image. When you input an empty imageData object, the cropped image is saved to the object. If it is not a empty imageData object, the existing image data object is destroyed then replaced with the cropped image.
			* @param roi area to crop
			*/
			static Status __stdcall CropImage(const ImageData& src, ImageData& dst, Rect roi);

			/** @brief Undocumented function*/
			static Status GetFrequencyDistribution(const FloatArray& inputArr, SuaKIT_UInt64 classN, float minVal, float maxVal, SuaKIT_UInt64 ranks, Int64Array& resultFreqDist);
			/** @brief Undocumented function*/
			static Status GetMaxScore(const FloatArray & inputArr, const SuaKIT_UInt64 classN, const SuaKIT_UInt64 totalClassN, Int64Array& resultMaxScore);
			/** @brief Undocumented function*/
			static Status __stdcall TurnOnLogger();
			/** @brief Undocumented function*/
			static Status __stdcall TurnOnConsoleLogger();
			/** @brief Undocumented function*/
			static Status __stdcall SuaKIT_ExtraFacilitiesInit();
			/** @brief Undocumented function*/
			static Status __stdcall isImportableModel(ModelType modelType, SuaKIT_Int32 imgCnt, SuaKIT_Int32 numberOfClasses, const wchar_t *modelPath);
            
            /**
            * @brief Returns the version of SuaKIT.
            * @author ⓒSualab SuaKIT Team
            */
            static SuaKIT_Version __stdcall SuaKIT_GetVersion();

			/**
			* @brief Get the information of trained model.
			* @param trainedModelInfo Information of trained model.
			* @param modelPath The trained model path for executing GetTrainedModelInfo.
			* @return Returns Status.
			* @author ⓒSualab SuaKIT Team
			*/
			static Status __stdcall GetTrainedModelInfo(TrainedModelInfo& trainedModelInfo, const wchar_t* modelPath);

			/**
			* @brief Get the information of trained model.
			* @param modelPath The trained model path
			* @return Message objects holding class names. You can get the wchar_t pointer of the string of the class names using Getdata method of returned object.
			   The string is a json list. Each entry in the json list represents the name of the class and is represented sequentially from class 0.
			* @code	Message msg = UnitFunction::GetAllClassNames(L"./model.net");
			   const wchar_t* wstr = msg.Getdata();
			   std::wstring classNames(wstr); // ["className1","className2","className3"] json list format
			  @endcode
			* @author ⓒSualab SuaKIT Team
			*/
			static Message __stdcall GetAllClassNames(const wchar_t* modelPath);

			/**
			* @brief Get the maximum image size information
			* @param gpuFreeMemSize GPU free memory size returned by GetGpuFreeMemorySize() API.
			* @param modelType This parameter is used to configure model type. Set it to one of {ModelType::CLASSIFICATION, ModelType::DETECTION, ModelType::DETECTION_DRAGON, ModelType::DETECTION_DOLPHIN, }
			* @param inputDataType This parameter is used to configure input type of neural network. For now, just set it to ModelType::CLASSIFICATION.
			* @param imgCnt The number of images for Evaluator, depends on inputDataType parameter.
			- In case of InputDataType::SINGLE, multiImgCount parameter needs to initialized with 1.
			- In case of InputDataType::COMPARISON, multiImgCount parameter needs to initialized with 2.
			- In case of InputDataType::MULTIMG, multiImgCount parameter needs to initialized with imageNum >= 2.
			* @param numClass The number of classes to train.
			* @param modelCapacity The base (pre-defined) model capacity for executing ClassificationTrainer. {0, 1, 2, 3}.
			- 0 means small model capacity.
			- 1 means normal model capacity.
			- 2 means large model capacity.
			- 3 means extra large model capacity.
			* @param widthHeightRatio The ratio of width to height of images in train set. It affects the return value width and height.
			* @param width It returns the width of the maximum network size.
			* @param height It returns the height of the maximum network size.
			* @return Returns Status.
			* @code .
			* @endcode
			* @author ⓒSualab SuaKIT Team
			*/
			static Status __stdcall GetMaxNetworkSize(SuaKIT_UInt64 gpuFreeMemSize, ModelType modelType, InputDataType inputDataType, SuaKIT_Int32 imgCnt, SuaKIT_Int32 numClass, SuaKIT_Int32 modelCapacity, float widthHeightRatio, SuaKIT_Int32& width, SuaKIT_Int32& height);

			/**
			* @brief Get free memory size in byte of a GPU
			* @param devDesc Specifies the device on which the trainer will operate.
			* @param gpuFreeMemSize It returns the free GPU memory size of the GPU specified by devDesc in bytes.
			* @return Returns Status.
			* @code .
			* @endcode
			* @author ⓒSualab SuaKIT Team
			*/
			static Status __stdcall GetGpuFreeMemorySize(DeviceDescriptor devDesc, SuaKIT_UInt64& gpuFreeMemSize);

			/**
			* @brief You can check whether this model is possible to continue training or not.
			* @param modelType Model type for continue training mode.
			Currently, ModelType::CLASSIFICATION is supported only.
			* @param imgCnt The number of images for continue training mode, depends on inputDataType.
			- In case of InputDataType::SINGLE, imgCnt parameter needs to be initialized with 1.
			- In case of InputDataType::COMPARISON, imgCnt parameter needs to be initialized with 2.
			- In case of InputDataType::MULTIMG, imgCnt parameter needs to be initialized with imgCnt >= 2.
			* @param numberOfClasses The number of classes to continue training mode.
			* @param modelPath The trained model path for executing isContinueTrainableBaseModel.
			* @return Returns Status.
			* @author ⓒSualab SuaKIT Team
			*/
			static Status __stdcall isContinueTrainableBaseModel(ModelType modelType, SuaKIT_Int32 imgCnt, SuaKIT_Int32 numberOfClasses, const wchar_t *modelPath);

		private:
			UnitFunction();
			UnitFunction(const UnitFunction & src);
			UnitFunction& operator=(const UnitFunction& rhs);
		};

		/**
		* @brief This class manages the information for the device to be used by the SuaKIT engine. The static functions return information regarding the usable devices on the PC, and each object is allocated one device.
		* @details
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API DeviceDescriptor {
		public:
			DeviceDescriptor();

			/**
			* @brief Allocates a specific device to an object.
			* @param deviceId Input for the ID index integer for the device to be allocated to the object.
			* @param deviceKind Input for the type of device to be allocated to the object.
			*/
			DeviceDescriptor(SuaKIT_UInt64 deviceId, DeviceKind deviceKind);

			/** @brief Returns the CPU DeviceDescriptor.*/
			static DeviceDescriptor __stdcall GetCPUDevice();
			/** @brief Put all GPU Descriptors in the devDescArray argument.*/
			static Status __stdcall GetAllGPUDevices(DeviceDescriptorArray& devDescArray);

			/**	@brief Put Available GPU Descriptors in the devDescArray argument.*/
			static Status __stdcall GetAvailableGPUDevices(DeviceDescriptorArray& devDescArray);

			/** @brief Returns the Default DeviceDescriptor.*/
			static DeviceDescriptor __stdcall GetDefaultDevice();

			/** @brief Returns the Device name.*/
			Message __stdcall GetDeviceName() const;

			/** @brief Undocumented function*/
			bool __stdcall isLocked();

			/** @brief Returns the device id integer for the current object.*/
			SuaKIT_UInt64 GetDeviceId() const;

			/** @brief Returns the device type for the current object.*/
			DeviceKind GetDeviceKind() const;

			/** @brief Returns the device type for the current object in string.*/
			Message __stdcall GetDeviceKindStr() const;

			/**
			* @brief Returns the amount of free GPU memory in bytes.
			*
			* This function is useful only when the device is SuaKIT::API::DeviceKind_GPU.
			*
			* @return The amount of free memory on this device in bytes.
			*         Returns 0 if the device is not SuaKIT::API::DeviceKind_GPU.
			*/
			SuaKIT_UInt64 GetFreeGpuMem() const;
		private:
			SuaKIT_UInt64 m_deviceId;
			DeviceKind m_deviceKind;
		};

		/**
		* @brief Undocumented class.
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API Internal {
		public:
			/** @brief Undocumented function*/
			static void* __stdcall SuaKIT_Malloc(SuaKIT_UInt64 byteSize);
			/** @brief Undocumented function*/
			static void __stdcall SuaKIT_Free(void* m_startp);
		private:
			Internal();
			Internal(const Internal & src);
			Internal& operator=(const Internal& rhs);
		};

		/**
		* @brief Classes for kmeans calculations. It is used to determine the value of an anchor box in DetectionTrainConfig based on training data.
		* @details
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API kmeans2d {
		public:
			enum ResultOpt{
				NON = 0,
				CLUSTOR_PRIOR = 1, /**<  Set cluster value alignment to cluster priority  ex: (X1,Y1), (X2,Y2), (X3,Y3) */
				DIM_PRIOR = 2  /**< Set cluster value alignment to dimension priority  ex: (X1,X2,X3), (Y1,Y2,Y3)*/
			};
			__stdcall kmeans2d();
			__stdcall ~kmeans2d();

			SuaKIT_UInt64 __stdcall GetItemLength() const;
			/** @brief Input data for kmeans */
			void __stdcall Push2dItem(const float x, const float y);
			/** @brief Run kmeans calculations. The center coordinates of the clusters are returned in resultCenters.*/
			Status __stdcall CalcKmeans(const SuaKIT_UInt64 clusterCnt, FloatArray& resultCenters, ResultOpt resOpt = ResultOpt::NON);

		private:
			kmeans2d(const kmeans2d & src); //=delete;
			kmeans2d& operator=(const kmeans2d& rhs); //=delete;
			void *internal;

		};

		template<typename VAL_TYPE>
		inline DataArray<VAL_TYPE>::DataArray(SuaKIT_UInt64 size)
		{
			m_startp = Internal::SuaKIT_Malloc(size * sizeof(VAL_TYPE));
			m_byteSize = size * sizeof(VAL_TYPE);
			m_capacity = size * sizeof(VAL_TYPE);
		}

		template<typename VAL_TYPE>
		inline DataArray<VAL_TYPE>::~DataArray()
		{
			Internal::SuaKIT_Free(m_startp);
		}
		template<typename VAL_TYPE>
		inline const VAL_TYPE & DataArray<VAL_TYPE>::GetAt(SuaKIT_UInt64 idx) const
		{
			return (reinterpret_cast<VAL_TYPE*>(m_startp))[idx];
		}

		template<typename VAL_TYPE>
		inline VAL_TYPE DataArray<VAL_TYPE>::GetValue(SuaKIT_UInt64 idx) const
		{
			return (reinterpret_cast<VAL_TYPE*>(m_startp))[idx];
		}

		template<typename VAL_TYPE>
		inline SuaKIT_UInt64 DataArray<VAL_TYPE>::GetByteSize() const
		{
			return m_byteSize;
		}

		template<typename VAL_TYPE>
		inline SuaKIT_UInt64 DataArray<VAL_TYPE>::GetLength() const
		{
			return m_byteSize / sizeof(VAL_TYPE);
		}

		template<typename VAL_TYPE>
		inline VAL_TYPE * DataArray<VAL_TYPE>::GetDataPtr()
		{
			return reinterpret_cast<VAL_TYPE*>(m_startp);
		}

		template<typename VAL_TYPE>
		inline const VAL_TYPE * DataArray<VAL_TYPE>::GetDataPtr() const
		{
			return reinterpret_cast<VAL_TYPE*>(m_startp);
		}

		template<typename VAL_TYPE>
		inline Status DataArray<VAL_TYPE>::SetAt(SuaKIT_UInt64 idx, const VAL_TYPE & item)
		{
			if (idx >= (m_byteSize / sizeof(VAL_TYPE)))
				return SuaKIT::API::Status::ERR_INPUT;
			(reinterpret_cast<VAL_TYPE*>(m_startp))[idx] = item;
			return SuaKIT::API::Status::SUCCESS;
		}

		inline DeviceDescriptor::DeviceDescriptor()
			: m_deviceId(0), m_deviceKind(DeviceKind::DeviceKind_NON)
		{
		}

		inline DeviceDescriptor::DeviceDescriptor(SuaKIT_UInt64 deviceId, DeviceKind deviceKind)
			: m_deviceId(deviceId), m_deviceKind(deviceKind)
		{
		}

		inline SuaKIT_UInt64 DeviceDescriptor::GetDeviceId() const
		{
			return m_deviceId;
		}

		inline DeviceKind DeviceDescriptor::GetDeviceKind() const
		{
			return m_deviceKind;
		}

		template<typename VAL_TYPE>
		inline void DataArray<VAL_TYPE>::_resize(SuaKIT_UInt64 size)
		{
			if (static_cast<SuaKIT_UInt64>(m_capacity) < (size * sizeof(VAL_TYPE))) {
				Internal::SuaKIT_Free(m_startp);
				m_startp = Internal::SuaKIT_Malloc(size * sizeof(VAL_TYPE));
				m_byteSize = size * sizeof(VAL_TYPE);
				m_capacity = m_byteSize;
			}
			m_byteSize = size * sizeof(VAL_TYPE);
		}
	}
}
