#pragma once
/*
SuaKIT runtime API 2.2.4.11
Copyright (c) 2018 SuaLab, All right reserved.
*/
/**
* @file SuaKITRuntime.h
* @details This file contains the Evaluator functions that are used to integrate the SuaKIT into the actual production line. Evaluator is used to run test models created using the SuaKIT developer version or SuaKIT train API.
*/
#include "SuaKIT.h"

#ifdef SWIG
%apply float& INOUT{ float& resultUncertainty };
%apply float& INOUT{ float& resultScore };
%apply long long& INOUT{ SuaKIT_Int64& resultClass };
#endif

namespace SuaKIT {
	namespace API {
		
		/**
		* @brief This is an evaluator for classification.
		* @details A class used to apply the classification model trained in SuaKIT to a real runtime environment. The constructor
		* takes in the path of the classification model as an input and uses it to provide various runtime functions as member functions.
		* If a batchSize is set, the networks for 1 to batchSize must all be initialized, so it may take quite some time for the initialization to complete.
		* As batchSize increases, the throughput for batch operations increases, but latency may increase as well.
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API ClassificationEvaluator {

		public:
			/**
			* @brief A constructor for ClassificationEvaluator.
			* @param modelPath Input for the path of the classification model. As using backslash(`\`) characters
			* in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			* For example, `L"C:\Models\classification_model.net"` is preferred over `L"C:/Models/classification_model.net"`.
			* @param devDesc Input for the device descriptor to be used by the ClassificationEvaluator.
			* @param networkH Input in integers for the height of the images to be used by the ClassificationEvaluator. The height value must be 96 or greater. This value should be maximum height of images for evaluate.
			* @param networkW Input in integers for the width of the images to be used by the ClassificationEvaluator. The width value must be 96 or greater. This value should be maximum width of images for evaluate.
			* @param networkC Input in integers for the channel size of the images to be used by the ClassificationEvaluator. This value should be equal to channel of image at Evaluate function.
			* @param batchSize Batch Size. Specifies how many images are evaluated at once when calling the EvaluateWithBatch method once.
			* @param dataType This parameter is used to configure input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			* @param multiImgCount The number of images for Evaluator, depends on inputDataType parameter.
			- In case of InputDataType::SINGLE, multiImgCount parameter needs to initialized with 1.
			- In case of InputDataType::COMPARISON, multiImgCount parameter needs to initialized with 2.
			- In case of InputDataType::MULTIMG, multiImgCount parameter needs to initialized with imageNum >= 2.
			* @param warmingup In SuaKIT we have warming up (autotuning) for determining which convolution to be used during execution.
			Warming up consumes extra memory, thus in the first evaluation we use zero/low memory convolution methods to make sure data buffers are allocated first.
			Then during the second evaluation we perform warming up to find the fastest algorithm. Starting the third evaluation things will run very fast.
			If you assign true (default) into this parameter, then warming up is performed with ClassificationEvaluator initialization.
			If you assign false into this parameter, then warming up is performed with first two evaluation (Evaluate or EvaluateWithBatch).
			* @param isFixedNetworkSize This parameter is for internall use only. Ignore this parameter.
			* @param temperature TODO
			*/
			__stdcall ClassificationEvaluator(
				const wchar_t* modelPath, DeviceDescriptor devDesc, SuaKIT_UInt64 networkH, SuaKIT_UInt64 networkW, SuaKIT_UInt64 networkC, 
				SuaKIT_UInt64 batchSize = 1, InputDataType dataType = SINGLE , SuaKIT_UInt64 multiImgCount = 1, bool warmingup = true, bool isFixedNetworkSize = false,
				const float temperature = 1.0f);
			__stdcall ~ClassificationEvaluator();

			/**
			* @brief Uses the ImageData object to run classification runtime image evaluation.
			* @param image Provides an image object as a parameter for Evaluate.
			The size of the image must be identical to, or smaller than, the networkH and networkW values set by the constructor.
			Performance is best when the input images have identical values to the networkH and networkW values set by the constructor, while performance will decrease when the input images are smaller than those values.
			* @param resultClass Provides the SuaKIT_Int64 type as an integer that will receive the results of Evaluate. After the Evaluate member function terminates, the results are placed in the resultClass variable.
			* @param probThresholds Makes it so that the classification model's evaluation probability results are interpreted using these threshold values. The size of the probThresholds array must be identical to the number of classes in the classification model. In other words, you cannot assign a threshold for only one class.
			* @param maskImage Insert an array of mask images. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			* @return Returns Status.
			*/
			Status __stdcall Evaluate(
				const ImageData& image,
				SuaKIT_Int64& resultClass,
				const FloatArray& probThresholds = FloatArray(),
				const ImageData& maskImage = ImageData()
			);
			/**
			* @brief Uses the ImageData object to run classification runtime image evaluation.
			* @param image Provides an image object as a parameter for Evaluate.
			The size of the image must be identical to, or smaller than, the networkH and networkW values set by the constructor.
			Performance is best when the input images have identical values to the networkH and networkW values set by the constructor, while performance will decrease when the input images are smaller than these values.
			* @param resultClass Provides the SuaKIT_Int64 type as an integer that will receive the results of Evaluate. After the Evaluate member function terminates, the results are placed in the resultClass variable.
			* @param resultProbMap By providing the FloatArray type resultProbMap as a parameter, the class prediction probabilities calculated by the classification model can be received as an array.
			* @param resultUncertainty Provides result uncertainty value of given image. The range of this value is [0, 1]. 
			Lower uncertainty value means that the network is confident with the result class.
			Higher uncertainty value means that the network is ambiguous with the result class. 
			* @param probThresholds Makes it so that the classification model's evaluation probability results are interpreted using these threshold values. The size of the probThresholds array must be identical to the number of classes in the classification model. In other words, you cannot assign a threshold for only one class.
			* @param uncertaintyThresholds Makes it so that the classification model's evaluation uncertainty results are interpreted using these threshold values. The size of the uncertaintyThresholds array must be identical to the number of classes in the classification model. In other words, you cannot assign a threshold for only one class.
			If the uncertainty value is larger than the uncertainty threshold value of the predicted class, it is changed to the unknown class.
			* @param maskImage Insert an array of mask images. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			* @return Returns Status.
			*/
			Status __stdcall Evaluate(
				const ImageData& image,
				SuaKIT_Int64& resultClass,
				FloatArray& resultProbMap,
				float& resultUncertainty,
				const FloatArray& probThresholds = FloatArray(),
				const FloatArray& uncertaintyThresholds = FloatArray(),
				const ImageData& maskImage = ImageData()
			);
			/**
			* @brief Uses the ImageData object to run classification runtime image evaluation.
			* @param image Provides an image object as a parameter for Evaluate.
			The size of the image must be identical to, or smaller than, the networkH and networkW values set by the constructor.
			Performance is best when the input images have identical values to the networkH and networkW values set by the constructor, while performance will decrease when the input images are smaller than these values.
			* @param resultClass Provides the SuaKIT_Int64 type as an integer that will receive the results of Evaluate. After the Evaluate member function terminates, the results are placed in the resultClass variable.
			* @param resultProbMap By providing the FloatArray type resultProbMap as a parameter, the class prediction probabilities calculated by the classification model can be received as an array.
			* @param resultUncertainty Provides result uncertainty value of given image. The range of this value is [0, 1].
			Lower uncertainty value means that the network is confident with the result class.
			Higher uncertainty value means that the network is ambiguous with the result class.
			* @param debugProbMap The debugProbMap argument is an image that represents the location of the feature used to classify the evaluated image in the deep learning network into that class.
			Calling the Evaluate method with the debugProbMap argument increases the evaluation time.
			* @param probThresholds Makes it so that the classification model's evaluation probability results are interpreted using these threshold values. The size of the probThresholds array must be identical to the number of classes in the classification model. In other words, you cannot assign a threshold for only one class.
			* @param uncertaintyThresholds Makes it so that the classification model's evaluation uncertainty results are interpreted using these threshold values. The size of the uncertaintyThresholds array must be identical to the number of classes in the classification model. In other words, you cannot assign a threshold for only one class.
			If the uncertainty value is larger than the uncertainty threshold value of the predicted class, it is changed to the unknown class.
			* @param maskImage Insert an array of mask images. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			* @return Returns Status.
			*/
			Status __stdcall Evaluate(
				const ImageData& image,
				SuaKIT_Int64& resultClass,
				FloatArray& resultProbMap,
				float& resultUncertainty,
				ImageData& debugProbMap,
				const FloatArray& probThresholds = FloatArray(),
				const FloatArray& uncertaintyThresholds = FloatArray(),
				const ImageData& maskImage = ImageData()
			);

			/**
			* @brief Calculates and returns the maximum batch size for the current ClassificationEvaluator model.
			* @param networkPath Input for the path of the classification model. As using backslash(`\`) characters
			* in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			* For example, `L"C:/Models/classification_model.net"` is preferred over `L"C:\\Models\\classification_model.net"`.
			* @param devDesc Input for the device descriptor to be used by the ClassificationEvaluator.
			* @param height Input in integers for the height of the images to be used by the ClassificationEvaluator. The height value must be 96 or greater.
			* @param width Input in integers for the width of the images to be used by the ClassificationEvaluator. The width value must be 96 or greater.
			* @param channel Input in integers for the channel size of the images to be used by the ClassificationEvaluator.
			* @return Returns Calculates and returns the maximum batch size for the current ClassificationEvaluator model.
			*/
			static SuaKIT_UInt64 __stdcall GetMaxBatchSize(const wchar_t* networkPath, DeviceDescriptor devDesc, SuaKIT_UInt64 height, SuaKIT_UInt64 width, SuaKIT_UInt64 channel);

			/**
			* @brief Calculates and returns the maximum network size for the current ClassificationEvaluator model.
			* @param networkPath Input for the path of the classification model. As using backslash(`\`) characters
			* in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			* For example `L"C:/Models/classification_model.net"` is preferred over `L"C:\\Models\\classification_model.net"`.
			* @param devDesc Input for the device descriptor to be used by the ClassificationEvaluator.
			* @return Returns Calculates and returns the maximum network size for the current ClassificationEvaluator model. Returned value refers to (networkH * networkW).
			Returned value 0 refers to error. Please check the error message.
			*/
			static SuaKIT_UInt64 __stdcall GetMaxNetworkSize(const wchar_t* networkPath, DeviceDescriptor devDesc);

			/**
			* @brief A member function that evaluates a bundle of images at once.
			* @param images Provides a const ImageArray& type object as a parameter for Evaluate. The size of the images array must be equal to or less than the batch size set by the constructor in order to run as intended.
			* @param resultClasses Provides as an integer the Int32Array& type that will receive the results of Evaluate. After the Evaluate member function terminates, the results are provided by the resultClasses variable.
			* @param probThresholds Makes it so that the classification model's evaluation probability results are interpreted using these threshold values.
			The size of the probThresholds array must be identical to the number of classes in the classification model. In other words, you cannot assign a threshold for only one class.
			* @param maskImages Insert an array of mask images. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			The size of the maskImages array should equal to the size of the images array that is first argument.
			* @return Returns Status.
			*/
			Status __stdcall EvaluateWithBatch(
				const ImageArray& images,
				Int32Array& resultClasses,
				const FloatArray& probThresholds = FloatArray(),
				const ImageArray& maskImages = ImageArray()
			);
			
			/**
			* @brief A member function that evaluates a bundle of images at once.
			* @param images Provides a const ImageArray& type object as a parameter for Evaluate. The size of the images array must be equal to or less than the batch size set by the constructor in order to run as intended.
			* @param resultClasses Provides as an integer the Int32Array& type that will receive the results of Evaluate. After the Evaluate member function terminates, the results are provided by the resultClasses variable.
			* @param resultProbMaps By providing the FloatArray type resultProbMap as a parameter, the class prediction probabilities calculated by the classification model can be received as an array. 
			The batch results that are given as an ImageArray are arranged in (image unit > class unit) order and saved to the resultProbMaps object. 
			For example, if two images are saved in images parameter and the EvaluateWithBatch member function is called for a classification model with 2 classes for classification, then the FloatArray& result will be in the order of [image 1's class 1 probability, image 1's class 2 probability, image 2's class 1 probability, image 2's class 2 probability].
			* @param resultUncertainties Provides result uncertainty values of given images as FloatArray type. Range of each value is [0, 1].
			Lower uncertainty value means that the network is confident with the result class.
			Higher uncertainty value means that the network is ambiguous with the result class.
			* @param probThresholds Makes it so that the classification model's evaluation probability results are interpreted using these threshold values.
			The size of the probThresholds array must be identical to the number of classes in the classification model. In other words, you cannot assign a threshold for only one class.
			* @param uncertaintyThresholds Makes it so that the classification model's evaluation uncertainty results are interpreted using these threshold values.
			The size of the uncertaintyThresholds array must be identical to the number of classes in the classification model. In other words, you cannot assign a threshold for only one class.
			If the uncertainty value is larger than the uncertainty threshold value of the predicted class, it is changed to the unknown class.
			* @param maskImages Insert an array of mask images. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			The size of the maskImages array should equal to the size of the images array that is first argument.
			* @return Returns Status.
			*/
			Status __stdcall EvaluateWithBatch(
				const ImageArray& images,
				Int32Array& resultClasses,
				FloatArray& resultProbMaps,
				FloatArray& resultUncertainties,
				const FloatArray& probThresholds = FloatArray(),
				const FloatArray& uncertaintyThresholds = FloatArray(),
				const ImageArray& maskImages = ImageArray()
			);

			/**
			* @brief A member function that evaluates a bundle of images at once.
			* @param images Provides a const ImageArray& type object as a parameter for Evaluate. The size of the images array must be equal to or less than the batch size set by the constructor in order to run as intended.
			* @param resultClasses Provides as an integer the Int32Array& type that will receive the results of Evaluate. After the Evaluate member function terminates, the results are provided by the resultClasses variable.
			* @param resultProbMaps By providing the FloatArray type resultProbMap as a parameter, the class prediction probabilities calculated by the classification model can be received as an array.
			The batch results that are given as an ImageArray are arranged in (image unit > class unit) order and saved to the resultProbMaps object.
			For example, if two images are saved in images parameter and the EvaluateWithBatch member function is called for a classification model with 2 classes for classification, then the FloatArray& result will be in the order of [image 1's class 1 probability, image 1's class 2 probability, image 2's class 1 probability, image 2's class 2 probability].
			* @param resultUncertainties Provides result uncertainty values of given images as FloatArray type. Range of each value is [0, 1].
			Lower uncertainty value means that the network is confident with the result class.
			Higher uncertainty value means that the network is ambiguous with the result class.
			* @param debugProbMaps The debugProbMap argument is an imageArray that represents the location of the feature used to classify the evaluated image in the deep learning network into that class.
			Calling the EvaluateWithBatch method with the debugProbMaps argument increases the evaluation time.
			* @param probThresholds Makes it so that the classification model's evaluation probability results are interpreted using these threshold values.
			The size of the probThresholds array must be identical to the number of classes in the classification model. In other words, you cannot assign a threshold for only one class.
			* @param uncertaintyThresholds Makes it so that the classification model's evaluation uncertainty results are interpreted using these threshold values.
			The size of the uncertaintyThresholds array must be identical to the number of classes in the classification model. In other words, you cannot assign a threshold for only one class.
			If the uncertainty value is larger than the uncertainty threshold value of the predicted class, it is changed to the unknown class.
			* @param maskImages Insert an array of mask images. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			The size of the maskImages array should equal to the size of the images array that is first argument.
			* @return Returns Status.
			*/
			Status __stdcall EvaluateWithBatch(
				const ImageArray& images,
				Int32Array& resultClasses,
				FloatArray& resultProbMaps,
				FloatArray& resultUncertainties,
				ImageArray& debugProbMaps,
				const FloatArray& probThresholds = FloatArray(),
				const FloatArray& uncertaintyThresholds = FloatArray(),
				const ImageArray& maskImages = ImageArray()
			);

			/** @brief Undocumented method*/
			Status __stdcall EvaluateWithBatch(
				const ImageArray& images,
				const wchar_t* targetNodeName,
				FloatArray& resultFeatureMaps
			);

			/** @brief Returns as an integer the total number of classes for the model that is allocated to the ClassificationEvaluator. */
			SuaKIT_UInt64 __stdcall GetClassTotalNum() const;
			/** @brief Returns the class name at the specified index position specified during training. */
			const char* __stdcall GetClassLabelName(SuaKIT_UInt64 i) const;


			/** @brief Returns the DeviceDescriptor that is allocated to the ClassificationEvaluator. */
			DeviceDescriptor __stdcall GetDeviceDescriptor() const;
			/** @brief Returns the id of the device that is allocated to the ClassificationEvaluator. */
			SuaKIT_UInt64 __stdcall GetDeviceId() const;
			/** @brief Returns the type of the device that is allocated to the ClassificationEvaluator. */
			DeviceKind __stdcall GetDeviceKind() const;

			/** @brief Returns the internal status code for the object.*/
			Status __stdcall GetStatus() const;

			/** @brief Returns as an integer networkH for the classification model that is allocated to the ClassificationEvaluator.*/
			SuaKIT_UInt64 GetNetworkH() const { return m_networkH; };
			/** @brief Returns as an integer networkW for the classification model that is allocated to the ClassificationEvaluator.*/
			SuaKIT_UInt64 GetNetworkW() const { return m_networkW; };
			/** @brief Returns as an integer networkC for the classification model that is allocated to the ClassificationEvaluator.*/
			SuaKIT_UInt64 GetNetworkC() const { return m_networkC; };
			/** @brief Returns as an integer the batch size for the classification model that is allocated to the ClassificationEvaluator.*/
			SuaKIT_UInt64 GetBatchSize() const { return m_batchSize; };
			/** @brief Returns as Number of images for Evaluator, depends on inputDataType*/
			SuaKIT_UInt64 GetMultiImageCount() const { return m_multiImgCount; };
			/** @brief Returns the InputDataType of the model.*/
			InputDataType __stdcall GetDataType() const { return m_dataType; };
#ifndef SWIG
			/** @brief The explicit resource cleanup method to terminate the ClassificationEvaluator. It is exactly the same as Destroy().*/
			Status __stdcall Finalize();
#endif
			/** @brief The explicit resource cleanup method to terminate the ClassificationEvaluator. It is exactly the same as Finalize().*/
			Status __stdcall Destroy();

		private:
			Status __stdcall _Evaluate(const ImageData& image, const FloatArray& probThresholds, const FloatArray& uncertaintyThresholds, const ImageData& maskImage, SuaKIT_Int64& resultClass, FloatArray& resultProbMap, float& resultUncertainty, ImageData& debugInfo, SuaKIT_UInt64 option);
			Status __stdcall _EvaluateWithBatch(const ImageArray& images, const FloatArray& probThresholds, const FloatArray& uncertaintyThresholds, const ImageArray& maskImages, Int32Array& resultClasses, FloatArray& resultProbMaps, FloatArray& resultUncertainties, ImageArray& debugProbMaps, SuaKIT_UInt64 option);
			
			ClassificationEvaluator(const ClassificationEvaluator & src); //=delete;
			ClassificationEvaluator& operator=(const ClassificationEvaluator& rhs); //=delete;
			SuaKIT_UInt64 m_networkH;
			SuaKIT_UInt64 m_networkW;
			SuaKIT_UInt64 m_networkC;
			SuaKIT_UInt64 m_multiImgCount;
			SuaKIT_UInt64 m_batchSize;

			DeviceDescriptor m_devDesc;
			SuaKIT_UInt64 m_classNum;
			void* internal;
			Status m_status;
			InputDataType m_dataType;
		};

		/**
		* @brief This is an evaluator for segmentation.
		* @details A class used to apply the segmentation model trained in SuaKIT to a real runtime environment. 
		The constructor takes in the path of the segmentation model as an input and uses it to provide various runtime functions as member functions.
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API SegmentationEvaluator {
		public:
			/**
			* @brief A constructor for SegmentationEvaluator.
			* @param modelPath Input for the path of the segmentation model. As using backslash(`\`) characters
			* in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			* For example `L"C:/Models/segmentation_model.net"` is preferred over `L"C:\Models\segmentation_model.net"`.
			* @param devDesc Input for the device descriptor to be used by the SegmentationEvaluator.
			* @param networkH Input in integers for the height of the network to be used by the SegmentationEvaluator. The height value must be 96 or greater. If the input image height is small than this value, then input image will be padded up to this size. And if the input image height is bigger than this value, then multiple evaluation will occur with sliding window manner. To calculate optimal networkH for tack time, see the API Guide doc.
			* @param networkW Input in integers for the width of the network to be used by the SegmentationEvaluator. The width value must be 96 or greater. If the input image width is small than this value, then input image will be padded up to this size. And if the input image width is bigger than this value, then multiple evaluation will occur with sliding window manner. To calculate optimal networkW for tack time, see the API Guide doc.
			* @param networkC Input in integers for the channel size of the images to be used by the SegmentationEvaluator. This value should be equal to channel of image at Evaluate function.
			* @param dataType This parameter is used to configure input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			* @param multiImgCount The number of images for Evaluator, depends on inputDataType parameter.
			- In case of InputDataType::SINGLE, multiImgCount parameter needs to initialized with 1.
			- In case of InputDataType::COMPARISON, multiImgCount parameter needs to initialized with 2.
			- In case of InputDataType::MULTIMG, multiImgCount parameter needs to initialized with imageNum >= 2.
			* @param warmingup In SuaKIT we have warming up (autotuning) for determining which convolution to be used during execution.
			Warming up consumes extra memory, thus in the first evaluation we use zero/low memory convolution methods to make sure data buffers are allocated first.
			Then during the second evaluation we perform warming up to find the fastest algorithm. Starting the third evaluation things will run very fast.
			If you assign true (default) into this parameter, then warming up is performed with SegmentationnEvaluator initialization.
			If you assign false into this parameter, then warming up is performed with first two evaluation (Evaluate).
			*/
			__stdcall SegmentationEvaluator(
				const wchar_t* modelPath,
				DeviceDescriptor devDesc,
				SuaKIT_UInt64 networkH,
				SuaKIT_UInt64 networkW,
				SuaKIT_UInt64 networkC,
				InputDataType dataType = SINGLE,
				SuaKIT_UInt64 multiImgCount = 1,
				bool warmingup = true
			);
			
			__stdcall ~SegmentationEvaluator();

			/**
			* @brief Uses the ImageData object to run segmentation runtime image evaluation.
			* @param image Provides an image object as a parameter for Evaluate.
			The size of the image must be identical to, or greater than, the networkH and networkW values set by the constructor.
			Performance is best when the input images have identical values to the networkH and networkW values set by the constructor, while performance will decrease when the input images are larger than these values.
			* @param resultImage Provides an object of the ImageData& type that will receive the results of Evaluate.
			Once the Evaluate member function terminates, the image that results from evaluating the image object with the segmentation model is saved in the resultImage object.
			The result image is stored in pixels from 0 to the class specified in the GUI. The unlabeled area of the GUI is stored as a 255 value.
			Example: first class mapped to pixel number 0, second class mapped to pixel number 1...
			* @param minDetectSizes Can set the minimum class size when calling Evaluate.
			The size of the minDetectSizes array must be identical to the number of classes in the segmentation model.
			* @param probThresholds Makes it so that the segmentation model's evaluation probability results are interpreted using these threshold values.
			The size of the probThresholds array must be identical to the number of classes in the segmentation model. In other words, you cannot assign a threshold for only one class.
			* @param maskImage Insert the mask image. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			* @param minRectWorkANDCondition Gives additional option on how the \p minDetectSizes parameter is interpreted. If set to true, the min. width condition and min. height condition are joined with AND operator. If set to false, the two conditions are joined with OR operator. If left empty, the default operator applied is AND operator.
			* @return Returns Status.
			*/
			Status __stdcall Evaluate(
				const ImageData& image,
				ImageData& resultImage,
				const SizeArray& minDetectSizes = SizeArray(),
				const FloatArray& probThresholds = FloatArray(),
				const ImageData& maskImage = ImageData(),
				const BoolArray& minRectWorkANDCondition = BoolArray()
			);

			/**
			* @brief Uses the ImageData object to run segmentation runtime image evaluation.
			* @param image Provides an image object as a parameter for Evaluate.
			The size of the image must be identical to, or greater than, the networkH and networkW values set by the constructor.
			Performance is best when the input images have identical values to the networkH and networkW values set by the constructor, while performance will decrease when the input images are larger than these values.
			* @param resultImage Provides an object of the ImageData& type that will receive the results of Evaluate.
			Once the Evaluate member function terminates, the image that results from evaluating the image object with the segmentation model is saved in the resultImage object.
			The result image is stored in pixels from 0 to the class specified in the GUI. The unlabeled area of the GUI is stored as a 255 value.
			Example: first class mapped to pixel number 0, second class mapped to pixel number 1...
			* @param resultRects By providing as a parameter the resultRects object of the RectArray type, the locations of the classes that appear in the resultImage will be processed as Rect structures and returned together.
			* @param minDetectSizes Can set the minimum class size when calling Evaluate.
			The size of the minDetectSizes array must be identical to the number of classes in the segmentation model.
			* @param probThresholds Makes it so that the segmentation model's evaluation probability results are interpreted using these threshold values.
			The size of the probThresholds array must be identical to the number of classes in the segmentation model. In other words, you cannot assign a threshold for only one class.
			* @param maskImage Insert the mask image. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			* @param minRectWorkANDCondition Gives additional option on how the \p minDetectSizes parameter is interpreted. If set to true, the min. width condition and min. height condition are joined with AND operator. If set to false, the two conditions are joined with OR operator. If left empty, the default operator applied is AND operator.
			* @return Returns Status.
			*/
			Status __stdcall Evaluate(
				const ImageData& image,
				ImageData& resultImage,
				RectArray& resultRects,
				const SizeArray& minDetectSizes = SizeArray(),
				const FloatArray& probThresholds = FloatArray(),
				const ImageData& maskImage = ImageData(),
				const BoolArray& minRectWorkANDCondition = BoolArray()
			);

			/**
			* @brief Uses the ImageData object to run segmentation runtime image evaluation.
			* @param image Provides an image object as a parameter for Evaluate.
			The size of the image must be identical to, or greater than, the networkH and networkW values set by the constructor.
			Performance is best when the input images have identical values to the networkH and networkW values set by the constructor, while performance will decrease when the input images are larger than these values.
			* @param resultImage Provides an object of the ImageData& type that will receive the results of Evaluate.
			Once the Evaluate member function terminates, the image that results from evaluating the image object with the segmentation model is saved in the resultImage object.
			The result image is stored in pixels from 0 to the class specified in the GUI. The unlabeled area of the GUI is stored as a 255 value.
			Example: first class mapped to pixel number 0, second class mapped to pixel number 1...
			* @param resultRects By providing as a parameter the resultRects object of the RectArray type, the locations of the classes that appear in the resultImage will be processed as Rect structures and returned together.
			* @param resultProbMap The class prediction probabilities per pixel calculated by the segmentation model can be received as an float array. 
			Actual data alignment in memory is (C, H, W). C is dimension of the output probability vector. This is the same as the number of class. H is the same as the input image height. W is the same as the input image width.
			This feature takes additional tack time.
			* @param resultUncertaintyMap Provides result uncertainty value pixel by pixel. Each value per pixel is calculated from probability vector of that pixel. 
			Actual data alignment in memory is (H, W). H is the same as the input image height. W is the same as the input image width.
			The range of each value is [0, 1].
			* @param minDetectSizes Can set the minimum class size when calling Evaluate.
			The size of the minDetectSizes array must be identical to the number of classes in the segmentation model.
			* @param probThresholds Makes it so that the segmentation model's evaluation probability results are interpreted using these threshold values.
			The size of the probThresholds array must be identical to the number of classes in the segmentation model. In other words, you cannot assign a threshold for only one class.
			* @param uncertaintyThresholds Makes it so that the segmentation model's evaluation uncertainty results are interpreted using these threshold values.
			The size of the uncertaintyThresholds array must be identical to the number of classes in the segmentation model. In other words, you cannot assign a threshold for only one class.
			If the uncertainty value is larger than the uncertainty threshold value of the predicted class, it is changed to the unknown class.
			* @param maskImage Insert the mask image. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			* @param minRectWorkANDCondition Gives additional option on how the \p minDetectSizes parameter is interpreted. If set to true, the min. width condition and min. height condition are joined with AND operator. If set to false, the two conditions are joined with OR operator. If left empty, the default operator applied is AND operator.
			* @return Returns Status.
			*/
			Status __stdcall Evaluate(
				const ImageData& image,
				ImageData& resultImage,
				RectArray& resultRects,
				FloatArray& resultProbMap,
				FloatArray& resultUncertaintyMap,
				const SizeArray& minDetectSizes = SizeArray(),
				const FloatArray& probThresholds = FloatArray(),
				const FloatArray& uncertaintyThresholds = FloatArray(),
				const ImageData& maskImage = ImageData(),
				const BoolArray& minRectWorkANDCondition = BoolArray()
			);

			/**
			* @brief Calculates and returns the maximum network size for the current SegmentationEvaluator model.
			* @param networkPath Input for the path of the segmentation model. As using backslash(`\`) characters
			* in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			* For example `L"C:/Models/segmentation_model.net"` is preferred over `L"C:\\Models\\segmentation_model.net"`.
			* @param devDesc Input for the device descriptor to be used by the SegmentationEvaluator.
			* @return Returns Calculates and returns the maximum network size for the current SegmentationEvaluator model. Returned value refers to both networkH * networkW.
			Returned value 0 refers to error. Please check the error message.
			*/
			static SuaKIT_UInt64 __stdcall GetMaxNetworkSize(const wchar_t* networkPath, DeviceDescriptor devDesc);


			/** @brief Undocumented method*/
			Message __stdcall GetContourJson();

			/** @brief Undocumented method*/
			Status __stdcall EvaluateForConfMat(
				const ImageData& image,
				const wchar_t * labelJson,
				const SizeArray& minDetectSizes,
				const BoolArray& minRectWorkANDCondition,
				const FloatArray& probThresholds,
				const FloatArray& uncertaintyThresholds,
				const ImageData& maskImage,
				ImageData& resultImage,
				RectArray& resultRects,
				Int32Array & iouIndexList,
				FloatArray & iouList,
				Int32Array& confMat,
				FloatArray& iouPerClass,
				FloatArray& probMap,
				FloatArray& uncertaintyMap
			);

			/** @brief Returns as an integer the total number of classes for the model that is allocated to the SegmentationEvaluator.*/
			SuaKIT_UInt64 __stdcall GetClassTotalNum() const;
			/** @brief Returns the class name at the specified index position specified during training. */
			const char* __stdcall GetClassLabelName(SuaKIT_UInt64 i) const;
			/** @brief Returns the DeviceDescriptor that is allocated to the SegmentationEvaluator.*/
			DeviceDescriptor __stdcall GetDeviceDescriptor() const;
			/** @brief Returns the id of the device that is allocated to the SegmentationEvaluator.*/
			SuaKIT_UInt64 __stdcall GetDeviceId() const;
			/** @brief Returns the type of the device that is allocated to the SegmentationEvaluator.*/
			DeviceKind __stdcall GetDeviceKind() const;

			/** @brief Returns the internal status code for the object.*/
			Status __stdcall GetStatus() const;

#ifndef SWIG
			/** @brief The explicit resource cleanup method to terminate the SegmentationEvaluator. It is exactly the same as Destroy().*/
			Status __stdcall Finalize();
#endif
			/** @brief The explicit resource cleanup method to terminate the SegmentationEvaluator. It is exactly the same as Finalize().*/
			Status __stdcall Destroy();

			/** @brief Returns as an integer networkH for the segmentation model that is allocated to the SegmentationEvaluator.*/
			SuaKIT_UInt64 GetNetworkH() const { return m_networkH; };
			/** @brief Returns as an integer networkW for the segmentation model that is allocated to the SegmentationEvaluator.*/
			SuaKIT_UInt64 GetNetworkW() const { return m_networkW; };
			/** @brief Returns as an integer networkC for the segmentation model that is allocated to the SegmentationEvaluator.*/
			SuaKIT_UInt64 GetNetworkC() const { return m_networkC; };

			/** @brief Returns as Number of images for Evaluator, depends on inputDataType*/
			SuaKIT_UInt64 GetMultiImageCount() const { return m_multiImgCount; };
			/** @brief Returns the InputDataType of the model.*/
			InputDataType __stdcall GetDataType() const { return m_dataType; };
		private:
			Status __stdcall _Evaluate(
				const ImageData& image,
				const SizeArray& minDetectSizes,
				const FloatArray& probThresholds,
				const FloatArray& uncertaintyThresholds,
				const ImageData& maskImage,
				ImageData& resultImage,
				RectArray& resultRects,
				FloatArray& probMap,
				FloatArray& uncertaintyMap,
				const BoolArray& minRectWorkANDCondition,
				SuaKIT_UInt64 option
			);
			SegmentationEvaluator(const SegmentationEvaluator& src); //=delete;
			SegmentationEvaluator& operator=(const SegmentationEvaluator& rhs); //=delete;
			SuaKIT_UInt64 m_networkH;
			SuaKIT_UInt64 m_networkW;
			SuaKIT_UInt64 m_networkC;
			SuaKIT_UInt64 m_multiImgCount;
			DeviceDescriptor m_devDesc;
			SuaKIT_UInt64 m_classNum;
			void* internal;
			Status m_status;
			InputDataType m_dataType;
		};

		/**
		* @brief This is evaluator for detection.
		* @details A class used to apply the detection model trained in SuaKIT to a real runtime environment.
		The constructor takes in the path of the detection model as an input and uses it to provide various runtime functions as member functions.
		* If a batchSize is set, the networks for 1 to batchSize must all be initialized, so it may take quite some time for the initialization to complete.
		* As batchSize increases, the throughput for batch operations increases, but latency may increase as well.
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API DetectionEvaluator {
		public:
			/**
			* @brief A constructor for DetectionEvaluator.
			* @param modelPath Input for the path of the detection model. As using backslash(`\`) characters
			* in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			* For example, `L"C:/Models/detection_model.net"` is preferred over `L"C:\\Models\\detection_model.net"`.
			* @param devDesc Input for the device descriptor to be used by the DetectionEvaluator.
			* @param networkH Input in integers for the height of the images to be used by the DetectionEvaluator. The height value must be 96 or greater. This value should be maximum height of images for evaluate.
			* @param networkW Input in integers for the width of the images to be used by the DetectionEvaluator. The width value must be 96 or greater. This value should be maximum width of images for evaluate.
			* @param networkC Input in integers for the channel size of the images to be used by the DetectionEvaluator. This value should be equal to channel of image at Evaluate function.
			* @param batchSize Batch Size. Specifies how many images are evaluated at once when calling the EvaluateWithBatch method once.
			* @param dataType This parameter is used to configure input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			InputDataType::COMPARISON and InputDataType::MULTIMG are currently not supported. This features would be updated later.
			* @param multiImgCount The number of images for Evaluator, depends on inputDataType parameter.
			- In case of InputDataType::SINGLE, multiImgCount parameter needs to initialized with 1.
			- In case of InputDataType::COMPARISON, multiImgCount parameter needs to initialized with 2.
			- In case of InputDataType::MULTIMG, multiImgCount parameter needs to initialized with imageNum >= 2.
			* @param warmingup In SuaKIT we have warming up (autotuning) for determining which convolution to be used during execution.
			Warming up consumes extra memory, thus in the first evaluation we use zero/low memory convolution methods to make sure data buffers are allocated first.
			Then during the second evaluation we perform warming up to find the fastest algorithm. Starting the third evaluation things will run very fast.
			If you assign true (default) into this parameter, then warming up is performed with DetectionEvaluator initialization.
			If you assign false into this parameter, then warming up is performed with first two evaluation (Evaluate or EvaluateWithBatch).
			*/
			__stdcall DetectionEvaluator(
				const wchar_t* modelPath,
				DeviceDescriptor devDesc,
				SuaKIT_UInt64 networkH,
				SuaKIT_UInt64 networkW,
				SuaKIT_UInt64 networkC,
				SuaKIT_UInt64 batchSize = 1,
				InputDataType dataType = SINGLE,
				SuaKIT_UInt64 multiImgCount = 1,
				bool warmingup = true
			);

			__stdcall ~DetectionEvaluator();
			
			/**
			* @brief Uses the ImageData object to run detection runtime image evaluation.
			* @param image Provides an image object as a parameter for Evaluate.
			The size of the image must be identical to, or smaller than, the networkH and networkW values set by the constructor.
			Performance is best when the input images have identical values to the networkH and networkW values set by the constructor, while performance will decrease when the input images are smaller than these values.
			* @param resultRects Provides an DetRectArrayOfArray& type that will receive the results of Evaluate. After the Evaluate member function terminates, the results are provided by the resultRects object. Because this function is just single evaluation, length of array of array will be 1.
			* @param maskImage Insert the mask image. The mask image must be the same size as the input image and must be a 1-channel image.
			The pixel value of masking area should be 0 and the other area should be 255 value.
			* @param minRectSizes Can set the minimum Rect size when calling Evaluate.
			The size of the minRectSizes array must be identical to the number of classes in the detection model.
			* @param clsProbThresholds Makes it so that the detection model's evaluation probability results are interpreted using these threshold values.
			The size of the clsProbThresholds array must be identical to the number of classes in the detection model. In other words, you cannot assign a threshold for only one class.
			* @param clsUncertaintyThresholds Makes it so that the detection model's evaluation uncertainty results are interpreted using these threshold values.
			The size of the clsUncertaintyThresholds array must be identical to the number of classes in the detection model. In other words, you cannot assign a threshold for only one class.
			If the uncertainty value is larger than the uncertainty threshold value of the predicted class, it is changed to the unknown class.
			* @param minRectWorkANDCondition Filtering conditions for width and height of minRectSizes. True if the condition is AND, false if the condition is OR.
			The size of the minRectWorkANDCondition array must be identical to the number of classes in the detection model.
			* @return Returns Status.
			*/
			Status __stdcall Evaluate(
				const ImageData& image,
				DetRectArrayOfArray& resultRects,
				const ImageData& maskImage = ImageData(),
				const SizeArray& minRectSizes = SizeArray(),
				const FloatArray& clsProbThresholds = FloatArray(),
				const FloatArray& clsUncertaintyThresholds = FloatArray(),
				const BoolArray& minRectWorkANDCondition = BoolArray()
			);

			/**
			* @brief A member function that evaluates many images at once.
			* @param images Provides a const ImageArray& type object as a parameter for Evaluate.
			The size of the images array must be equal to or less than the batch size set by the constructor in order to run as intended.
			* @param resultRects Provides an DetRectArrayOfArray& type that will receive the results of Evaluate. After the Evaluate member function terminates, the results are provided by the resultRects object. Because this function is just batch evaluation, length of array of array will be equal to batchSize provided at Constructor.
			* @param maskImages Insert an array of mask image. The mask image must be the same size as the input image and must be a 1-channel image.
			The pixel value of masking area should be 0 and the other area should be 255 value.
			The size of the maskImages array should equal to the size of the images array that is first argument.
			* @param minRectSizes Can set the minimum Rect size when calling Evaluate.
			The size of the minRectSizes array must be identical to the number of classes in the detection model.
			* @param clsProbThresholds Makes it so that the detection model's evaluation probability results are interpreted using these threshold values.
			The size of the clsProbThresholds array must be identical to the number of classes in the detection model. In other words, you cannot assign a threshold for only one class.
			* @param clsUncertaintyThresholds Makes it so that the detection model's evaluation uncertainty results are interpreted using these threshold values.
			The size of the clsUncertaintyThresholds array must be identical to the number of classes in the detection model. In other words, you cannot assign a threshold for only one class.
			If the uncertainty value is larger than the uncertainty threshold value of the predicted class, it is changed to the unknown class.
			* @param minRectWorkANDCondition Filtering conditions for width and height of minRectSizes. True if the condition is AND, false if the condition is OR.
			The size of the minRectWorkANDCondition array must be identical to the number of classes in the detection model.
			* @return Returns Status.
			*/
			Status __stdcall EvaluateWithBatch(
				const ImageArray& images,
				DetRectArrayOfArray& resultRects,
				const ImageArray& maskImages = ImageArray(),
				const SizeArray& minRectSizes = SizeArray(),
				const FloatArray& clsProbThresholds = FloatArray(),
				const FloatArray& clsUncertaintyThresholds = FloatArray(),
				const BoolArray& minRectWorkANDCondition = BoolArray()
			);

			/* Undocumented Methods */
			Status __stdcall EvaluateForConfMat(
				const ImageData& image,
				const DetRectArrayOfArray& gtRects,
				DetRectArrayOfArray& resultRects,
				Int32Array& maxIouIndexList,
				FloatArray& maxIouList,
				Int32Array& confMat,
				FloatArray& iouPerClass,
				const ImageData& maskImage = ImageData(),
				const SizeArray& minRectSizes = SizeArray(),
				const FloatArray& clsProbThresholds = FloatArray(),
				const FloatArray& clsUncertaintyThresholds = FloatArray(),
				const float iouThreshold = -1,
				const float confidenceThreshold = -1,
				const BoolArray& minRectWorkANDCondition = BoolArray()
			);

			/* Undocumented Methods */
			Status __stdcall EvaluateForConfMatWithBatch(
				const ImageArray& images,
				const DetRectArrayOfArray& gtRects,
				DetRectArrayOfArray& resultRects,
				Int32Array& maxIouIndexList,
				FloatArray& maxIouList,
				Int32Array& confMat,
				FloatArray& iouPerClass,
				const ImageArray& maskImages = ImageArray(),
				const SizeArray& minRectSizes = SizeArray(),
				const FloatArray& clsProbThresholds = FloatArray(),
				const FloatArray& clsUncertaintyThresholds = FloatArray(),
				const float iouThreshold = -1,
				const float confidenceThreshold = -1,
				const BoolArray& minRectWorkANDCondition = BoolArray()
			);

			/**
			* @brief Calculates and returns the maximum batch size for the current DetectionEvaluator model. Takes up to 2 minutes. This integer is dependent on the user's hardware.
			* @param networkPath Input for the path of the detection model. As using backslash(`\`) characters
			* in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			* For example, `L"C:/Models/detection_model.net"` is preferred over `L"C:\\Models\\detection_model.net"`.
			* @param devDesc Input for the device descriptor to be used by the DetectionEvaluator.
			* @param height Input in integers for the height of the images to be used by the DetectionEvaluator. The height value must be at least 96.
			* @param width Input in integers for the width of the images to be used by the DetectionEvaluator. The width value must be at least 96.
			* @param channel Input in integers for the channel size of the images to be used by the DetectionEvaluator.
			* @return Returns Calculates and returns the maximum batch size for the current DetectionEvaluator model.
			*/
			static SuaKIT_UInt64 __stdcall GetMaxBatchSize(const wchar_t* networkPath, DeviceDescriptor devDesc, SuaKIT_UInt64 height, SuaKIT_UInt64 width, SuaKIT_UInt64 channel);

			/**
			* @brief Calculates and returns the maximum network size for the current DetectionEvaluator model. Takes up to 2 minutes. This value is dependent on the user's hardware.
			* @param networkPath Input for the path of the detection model. As using backslash(`\`) characters
			* in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			* For example `L"C:/Models/detection_model.net"` is preferred over `L"C:\\Models\\detection_model.net"`.
			* @param devDesc Input for the device descriptor to be used by the DetectionEvaluator.
			* @return Returns Calculates and returns the maximum network size for the current DetectionEvaluator model. Returned value refers to (networkH * networkW).
			Returned value 0 refers to error. Please check the error message.
			*/
			static SuaKIT_UInt64 __stdcall GetMaxNetworkSize(const wchar_t* networkPath, DeviceDescriptor devDesc);


			/** @brief Returns as an integer the total number of classes for the model that is allocated to the DetectionEvaluator.*/
			SuaKIT_UInt64 __stdcall GetClassTotalNum() const;
			/** @brief Returns the class name at the specified index position specified during training. */
			const char* __stdcall GetClassLabelName(SuaKIT_UInt64 i) const;
			/** @brief Returns the DeviceDescriptor that is allocated to the DetectionEvaluator.*/
			DeviceDescriptor __stdcall GetDeviceDescriptor() const;
			/** @brief Returns the id of the device that is allocated to the DetectionEvaluator.*/
			SuaKIT_UInt64 __stdcall GetDeviceId() const;
			/** @brief Returns the type of the device that is allocated to the DetectionEvaluator.*/
			DeviceKind __stdcall GetDeviceKind() const;
			/** @brief Returns the internal status code for the object.*/
			Status __stdcall GetStatus() const;

			/** @brief Returns as an integer networkH for the detection model that is allocated to the DetectionEvaluator.*/
			SuaKIT_UInt64 GetNetworkH() const { return m_networkH; };
			/** @brief Returns as an integer networkW for the detection model that is allocated to the DetectionEvaluator.*/
			SuaKIT_UInt64 GetNetworkW() const { return m_networkW; };
			/** @brief Returns as an integer networkC for the detection model that is allocated to the DetectionEvaluator.*/
			SuaKIT_UInt64 GetNetworkC() const { return m_networkC; };

			/** @brief Returns as an integer the batch size for the detection model that is allocated to the DetectionEvaluator.*/
			SuaKIT_UInt64 GetBatchSize() const { return m_batchSize; };

			/** @brief Returns the InputDataType of the model.*/
			InputDataType __stdcall GetDataType() const { return m_dataType; };
#ifndef SWIG
			/** @brief The explicit resource cleanup method to terminate the DetectionEvaluator. It is exactly the same as Destroy().*/
			Status __stdcall Finalize();
#endif
			/** @brief The explicit resource cleanup method to terminate the DetectionEvaluator. It is exactly the same as Finalize().*/
			Status __stdcall Destroy();

		private:
			Status __stdcall _Evaluate(
				const ImageData& image,
				DetRectArrayOfArray& resultRects,
				const ImageData& maskImage,
				const SizeArray& minRectSizes,
				const FloatArray& clsProbThresholds,
				const FloatArray& clsUncertaintyThresholds,
				const float iouThreshold,
				const float confidenceThreshold,
				const BoolArray& minRectWorkANDCondition,
				SuaKIT_UInt64 option
			);

			Status __stdcall _EvaluateWithBatch(
				const ImageArray& images,
				DetRectArrayOfArray& resultRects,
				const ImageArray& maskImages,
				const SizeArray& minRectSizes,
				const FloatArray& clsProbThresholds,
				const FloatArray& clsUncertaintyThresholds,
				const float iouThreshold,
				const float confidenceThreshold,
				const BoolArray& minRectWorkANDCondition,
				SuaKIT_UInt64 option
			);

			DetectionEvaluator(const DetectionEvaluator& src); //=delete;
			DetectionEvaluator& operator=(const DetectionEvaluator& rhs); //=delete;
			SuaKIT_UInt64 m_networkH;
			SuaKIT_UInt64 m_networkW;
			SuaKIT_UInt64 m_networkC;
			SuaKIT_UInt64 m_multiImgCount;
			SuaKIT_UInt64 m_batchSize;

			DeviceDescriptor m_devDesc;
			SuaKIT_UInt64 m_classNum;
			void* internal;
			Status m_status;
			InputDataType m_dataType;
		};

		/**
		* @brief The class supports asynchronous evaluation.
		* @details Supports asynchronous evaluation. Currently only classification is available. 
		Takes data for evaluation through the PushImageForEvaluate method and places it into the internal input queue; the internal thread continually reads and processes the data in the input queue then puts the results into the internal output queue. 
		The result data in the internal output queue can be retrieved through the GetResultOfEvaluation method.
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API QueuingContainer
		{
		public:
			/**
			* @brief A constructor for QueuingContainer.
			* @param classificationEvaluatorPtr Pointer(In c++) or Reference(In c#) input for the evaluator object that will conduct evaluation.
			* @param useDebugProbMap Evaluate debugImageProbmaps, which is the result of the visual debugger at evaluation time. Using the useDebugProbMap argument as true increases the evaluation time.
			*/
			__stdcall QueuingContainer(
				ClassificationEvaluator* classificationEvaluatorPtr,
				bool useDebugProbMap = false
			);
			__stdcall ~QueuingContainer();

			/**
			* @brief Places into the queue the image, and its associated image id, to be evaluated. 
			The internal thread will consecutively take data from the queue and evaluate it, then place the results in the internal results queue.
			* @param image Input for the image object to be evaluated.
			* @param imageId Input for the integer value of the id for the image to be used when retrieving the results. 
			As this value is for identifying the image that corresponds to each individual result when retrieving the results, these values would be able to distinguish each image.
			* @return Returns Status.
			*/
			Status __stdcall PushImageForEvaluate(
				const ImageData& image,
				SuaKIT_Int32 imageId
			);

			/**
			* @brief Retrieves all results currently in the internal results queue. 
			If all of the images pushed through the PushImageForEvaluate method have been evaluated, then the results array will have as many results as pushed images.
			However, if the evaluations are still taking place, then only the results which have been completed will be placed in the results array. 
			If there are no pushed images, then the function will terminate with the array staying in the empty state. 
			If results are retrieved through this function, then the results are deleted from the class object's internal results queue.
			* @param resultClasses An array object for the retrieval of the class information for the results from the results queue. 
			Retrieves all evaluation results from the internal results queue. The retrieved results are deleted from the internal queue. 
			Each of the result values in this array match 1:1 with the Id values of the resultIds that is the second factor.
			* @param resultIds An array object for the retrieval of the Id information for the results from the results queue. 
			Retrieves all evaluation results from the internal results queue. The retrieved results are deleted from the internal queue. 
			Each of the result values in this array match 1:1 with the class values of the resultClasses array that is the first factor.
			* @return Returns Status.
			*/
			Status __stdcall GetResultOfEvaluation(Int32Array& resultClasses, Int32Array& resultIds);


			/**
			* @brief Retrieves all results currently in the internal results queue. This method works properly when useDebugProbMap is true when creating a QueuingContainer.
			If all of the images pushed through the PushImageForEvaluate method have been evaluated, then the results array will have as many results as pushed images.
			However, if the evaluations are still taking place, then only the results which have been completed will be placed in the results array.
			If there are no pushed images, then the function will terminate with the array staying in the empty state.
			If results are retrieved through this function, then the results are deleted from the class object's internal results queue.
			* @param resultClasses An array object for the retrieval of the class information for the results from the results queue.
			Retrieves all evaluation results from the internal results queue. The retrieved results are deleted from the internal queue.
			Each of the result values in this array match 1:1 with the Id values of the resultIds that is the second factor.
			* @param resultIds An array object for the retrieval of the Id information for the results from the results queue.
			Retrieves all evaluation results from the internal results queue. The retrieved results are deleted from the internal queue.
			Each of the result values in this array match 1:1 with the class values of the resultClasses array that is the first factor.
			* @param resultDebugProbMaps The debugProbMap argument is an imageArray that represents the location of the feature used to classify the evaluated image in the deep learning network into that class.
			Calling the EvaluateWithBatch method with the debugProbMaps argument increases the evaluation time.
			* @return Returns Status.
			*/
			Status __stdcall GetResultOfEvaluation(Int32Array& resultClasses, Int32Array& resultIds, ImageArray& resultDebugProbMaps);


			/** @brief Returns as an integer the number of data in the input queue.*/
			SuaKIT_UInt64 __stdcall GetQueueLength() const;
			/** @brief Returns the internal status code for the object.*/
			Status __stdcall GetStatus() const;

#ifndef SWIG
			/** @brief The explicit resource cleanup method to terminate the ClassificationEvaluator. It is exactly the same as Destroy().*/
			Status __stdcall Finalize();
#endif
			/** @brief The explicit resource cleanup method to terminate the ClassificationEvaluator. It is exactly the same as Finalize().*/
			Status __stdcall Destroy();

		private:
			QueuingContainer(const QueuingContainer& src); //=delete;
			QueuingContainer& operator=(const QueuingContainer& rhs); //=delete;
			void* internal;
			Status m_status;
		};

		/**
		* @brief This is an evaluator for OneClassSegmentation.
		* @details A class used to apply the OneClassSegmentation model trained in SuaKIT to a real runtime environment.
		The constructor takes in the path of the OneClassSegmentation model as an input and uses it to provide various runtime functions as member functions.
		* @author ⓒSualab SuaKIT Team
		*/
		class SUAKIT_API OneClassSegmentationEvaluator {
		public:
			/**
			* @brief A constructor for OneClassSegmentationEvaluator.
			* @param modelPath Input for the path of the OneClassSegmentation model. As using backslash(`\`) characters
			* in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			* For example `L"C:/Models/OneClassSegmentation_model.net"` is preferred over `L"C:\Models\OneClassSegmentation_model.net"`.
			* @param devDesc Input for the device descriptor to be used by the OneClassSegmentationEvaluator.
			* @param networkH Input in integers for the height of the network to be used by the OneClassSegmentationEvaluator. The height value must be 96 or greater. If the input image height is small than this value, then input image will be padded up to this size. And if the input image height is bigger than this value, then multiple evaluation will occur with sliding window manner. To calculate optimal networkH for tack time, see the API Guide doc.
			* @param networkW Input in integers for the width of the network to be used by the OneClassSegmentationEvaluator. The width value must be 96 or greater. If the input image width is small than this value, then input image will be padded up to this size. And if the input image width is bigger than this value, then multiple evaluation will occur with sliding window manner. To calculate optimal networkW for tack time, see the API Guide doc.
			* @param networkC Input in integers for the channel size of the images to be used by the OneClassSegmentationEvaluator. This value should be equal to channel of image at Evaluate function. This value must be equal to imageChannel of OneClassSegmentationTrainConfig.
			* @param warmingup In SuaKIT we have warming up (autotuning) for determining which convolution to be used during execution.
			Warming up consumes extra memory, thus in the first evaluation we use zero/low memory convolution methods to make sure data buffers are allocated first.
			Then during the second evaluation we perform warming up to find the fastest algorithm. Starting the third evaluation things will run very fast.
			If you assign true (default) into this parameter, then warming up is performed with OneClassSegmentationEvaluator initialization.
			If you assign false into this parameter, then warming up is performed with first two evaluation (Evaluate).
			*/
			__stdcall OneClassSegmentationEvaluator(
				const wchar_t* modelPath,
				DeviceDescriptor devDesc,
				SuaKIT_UInt64 networkH,
				SuaKIT_UInt64 networkW,
				SuaKIT_UInt64 networkC,
				bool warmingup = true
			);

			__stdcall ~OneClassSegmentationEvaluator();

			/**
			* @brief Uses the ImageData object to run OneClassSegmentation runtime image evaluation.
			* @param image Provides an image object as a parameter for Evaluate.
			The size of the image must be identical to, or greater than, the patchSize value set by train parameter.
			* @param threshold This value is binary threshold to process resultImage. It has a slightly different meaning from the other threshold values. The smaller this value is, the larger the score value returned by resultScore will be. When this value is increased, the score value returned to resultScore becomes smaller overall.
			* @param kernelSize This value is used for calculating resultProbMap. resultProbMap is calculated by sliding windows manner from model result data. This value is window kernel size of that process.
			* @param strides This value is used for calculating resultProbMap. resultProbMap is calculated by sliding windows manner from model result data. This value is stride of window jumping of that process.
			* @param resultImage Provides an object of the ImageData& type that will receive the results of Evaluate.
			Once the Evaluate member function terminates, the image that results from evaluating the image object with the OneClassSegmentation model is saved in the resultImage object.
			The result image is binary image calculated from resultProbMap and stores value 0 as defect(oneClass) and 255 as normal.
			* @param resultScore Calculate and save the max value of resultProbMap.
			* @param minDetectSizes Can set the minimum class size when calling Evaluate.
			The size of the minDetectSizes array must be identical to the number of classes in the OneClassSegmentation model.
			* @param maskImage Insert the mask image. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			* @return Returns Status.
			*/
			Status __stdcall Evaluate(
				const ImageData& image,
				const float threshold,
				const Size& kernelSize,
				const SuaKIT_UInt64 strides,
				ImageData& resultImage,
				float& resultScore,
				const SizeArray& minDetectSizes = SizeArray(),
				const ImageData& maskImage = ImageData()
			);

			/**
			* @brief Uses the ImageData object to run OneClassSegmentation runtime image evaluation.
			* @param image Provides an image object as a parameter for Evaluate.
			The size of the image must be identical to, or greater than, the patchSize value set by train parameter.
			* @param threshold This value is binary threshold to process resultImage. It has a slightly different meaning from the other threshold values. The smaller this value is, the larger the score value returned by resultScore will be. When this value is increased, the score value returned to resultScore becomes smaller overall.
			* @param kernelSize This value is used for calculating resultProbMap. resultProbMap is calculated by sliding windows manner from model result data. This value is window kernel size of that process.
			* @param strides This value is used for calculating resultProbMap. resultProbMap is calculated by sliding windows manner from model result data. This value is stride of window jumping of that process.
			* @param resultImage Provides an object of the ImageData& type that will receive the results of Evaluate.
			Once the Evaluate member function terminates, the image that results from evaluating the image object with the OneClassSegmentation model is saved in the resultImage object.
			The result image is binary image calculated from resultProbMap and stores value 0 as defect(oneClass) and 255 as normal.
			* @param resultScore Calculate and save the max value of resultProbMap.
			* @param resultRects By providing as a parameter the resultRects object of the RectArray type, the locations of the classes that appear in the resultImage will be processed as Rect structures and returned together.
			* @param minDetectSizes Can set the minimum class size when calling Evaluate.
			The size of the minDetectSizes array must be identical to the number of classes in the OneClassSegmentation model.
			* @param maskImage Insert the mask image. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			* @return Returns Status.
			*/
			Status __stdcall Evaluate(
				const ImageData& image,
				const float threshold,
				const Size& kernelSize,
				const SuaKIT_UInt64 strides,
				ImageData& resultImage,
				float& resultScore,
				RectArray& resultRects,
				const SizeArray& minDetectSizes = SizeArray(),
				const ImageData& maskImage = ImageData()
			);

			/**
			* @brief Uses the ImageData object to run OneClassSegmentation runtime image evaluation.
			* @param image Provides an image object as a parameter for Evaluate.
			The size of the image must be identical to, or greater than, the patchSize value set by train parameter.
			* @param threshold This value is binary threshold to process resultImage. It has a slightly different meaning from the other threshold values. The smaller this value is, the larger the score value returned by resultScore will be. When this value is increased, the score value returned to resultScore becomes smaller overall.
			* @param kernelSize This value is used for calculating resultProbMap. resultProbMap is calculated by sliding windows manner from model result data. This value is window kernel size of that process.
			* @param strides This value is used for calculating resultProbMap. resultProbMap is calculated by sliding windows manner from model result data. This value is stride of window jumping of that process.
			* @param resultImage Provides an object of the ImageData& type that will receive the results of Evaluate.
			Once the Evaluate member function terminates, the image that results from evaluating the image object with the OneClassSegmentation model is saved in the resultImage object.
			The result image is binary image which stores value 0 as defect(oneClass) and 255 as normal.
			* @param resultScore Calculate and save the max value of resultProbMap.
			* @param resultRects By providing as a parameter the resultRects object of the RectArray type, the locations of the classes that appear in the resultImage will be processed as Rect structures and returned together.
			* @param resultProbMap The oneClass probabilities per pixel calculated by the OneClassSegmentation model can be received as an float array.
			This resultProbMap is calculated by sliding windows manner from model result data. And each pixel value is normalized by [0, 1].
			Actual data alignment in memory is (H, W). H is the same as the input image height. W is the same as the input image width.
			* @param minDetectSizes Can set the minimum class size when calling Evaluate.
			The size of the minDetectSizes array must be identical to the number of classes in the OneClassSegmentation model.
			* @param maskImage Insert the mask image. The mask image must be the same size as the input image and must be a 1-channel image. The pixel value of masking area should be 0 and the other area should be 255 value.
			* @return Returns Status.
			*/
			Status __stdcall Evaluate(
				const ImageData& image,
				const float threshold,
				const Size& kernelSize,
				const SuaKIT_UInt64 strides,
				ImageData& resultImage,
				float& resultScore,
				RectArray& resultRects,
				FloatArray& resultProbMap,
				const SizeArray& minDetectSizes = SizeArray(),
				const ImageData& maskImage = ImageData()
			);


			/**
			* @brief Calculates and returns the maximum network size for the current OneClassSegmentationEvaluator model. Takes up to 2 minutes. This value is dependent on the user's hardware.
			* @param networkPath Input for the path of the OneClassSegmentation model. As using backslash(`\`) characters
			* in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			* For example `L"C:/Models/OneClassSegmentation_model.net"` is preferred over `L"C:\\Models\\OneClassSegmentation_model.net"`.
			* @param devDesc Input for the device descriptor to be used by the OneClassSegmentationEvaluator.
			* @return Returns Calculates and returns the maximum network size for the current OneClassSegmentationEvaluator model. Returned value refers to both networkH and networkW(same value).
			Returned value 0 refers to error. Please check the error message.
			*/
			static SuaKIT_UInt64 __stdcall GetMaxNetworkSize(const wchar_t* networkPath, DeviceDescriptor devDesc);


			/** @brief Undocumented method*/
			Message __stdcall GetContourJson();

			/** @brief Undocumented method*/
			Status __stdcall EvaluateForConfMat(
				const ImageData& image,
				const wchar_t * labelJson,
				const SizeArray& minDetectSizes,
				const FloatArray& thresholds,
				const ImageData& maskImage,
				ImageData& resultImage,
				RectArray& resultRects,
				Int32Array & iouIndexList,
				FloatArray & iouList,
				Int32Array& confMat,
				FloatArray& iouPerClass,
				FloatArray& probMap
			);

			/** @brief Returns as an integer the total number of classes for the model that is allocated to the OneClassSegmentationEvaluator.*/
			SuaKIT_UInt64 __stdcall GetClassTotalNum() const;
			/** @brief Returns the class name at the specified index position specified during training. */
			const char* __stdcall GetClassLabelName(SuaKIT_UInt64 i) const;
			/** @brief Returns the DeviceDescriptor that is allocated to the OneClassSegmentationEvaluator.*/
			DeviceDescriptor __stdcall GetDeviceDescriptor() const;
			/** @brief Returns the id of the device that is allocated to the OneClassSegmentationEvaluator.*/
			SuaKIT_UInt64 __stdcall GetDeviceId() const;
			/** @brief Returns the type of the device that is allocated to the OneClassSegmentationEvaluator.*/
			DeviceKind __stdcall GetDeviceKind() const;

			/** @brief Returns the internal status code for the object.*/
			Status __stdcall GetStatus() const;

#ifndef SWIG
			/** @brief The explicit resource cleanup method to terminate the OneClassSegmentationEvaluator. It is exactly the same as Destroy().*/
			Status __stdcall Finalize();
#endif
			/** @brief The explicit resource cleanup method to terminate the OneClassSegmentationEvaluator. It is exactly the same as Finalize().*/
			Status __stdcall Destroy();

			/** @brief Returns as an integer networkH for the OneClassSegmentation model that is allocated to the OneClassSegmentationEvaluator.*/
			SuaKIT_UInt64 GetNetworkH() const { return m_networkH; };
			/** @brief Returns as an integer networkW for the OneClassSegmentation model that is allocated to the OneClassSegmentationEvaluator.*/
			SuaKIT_UInt64 GetNetworkW() const { return m_networkW; };
			/** @brief Returns as an integer networkC for the OneClassSegmentation model that is allocated to the OneClassSegmentationEvaluator.*/
			SuaKIT_UInt64 GetNetworkC() const { return m_networkC; };

			/** @brief Returns as Number of images for Evaluator, depends on inputDataType*/
			SuaKIT_UInt64 GetMultiImageCount() const { return m_multiImgCount; };
			/** @brief Returns the InputDataType of the model.*/
			InputDataType GetDataType() const { return m_dataType; };
		private:
			Status __stdcall _Evaluate(
				const ImageData& image,
				const float threshold,
				const SizeArray& minDetectSizes,
				const ImageData& maskImage,
				const Size& kernelSize,
				const SuaKIT_UInt64 strides,
				ImageData& resultImage,
				float& resultScore,
				RectArray& resultRects,
				FloatArray& resultProbMap,
				SuaKIT_UInt64 option
			);
			OneClassSegmentationEvaluator(const OneClassSegmentationEvaluator& src); //=delete;
			OneClassSegmentationEvaluator& operator=(const OneClassSegmentationEvaluator& rhs); //=delete;
			SuaKIT_UInt64 m_networkH;
			SuaKIT_UInt64 m_networkW;
			SuaKIT_UInt64 m_networkC;
			SuaKIT_UInt64 m_multiImgCount;
			DeviceDescriptor m_devDesc;
			SuaKIT_UInt64 m_classNum;
			void* internal;
			Status m_status;
			InputDataType m_dataType;
		};

	}
}