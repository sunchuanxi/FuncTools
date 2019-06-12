#pragma once
/*
SuaKIT runtime API 2.0.0
Copyright (c) 2017 SuaLab, All right reserved.
*/
/**
* @file SuaKITTrainer.h
*/
#include "SuaKIT.h"

namespace SuaKIT {
	namespace API {
		/**
		* @brief This is a configuration of augmentation in training time.
		* @details It is used as data augmentation setting parameter for Trainer classes.
		* @author ?Sualab SuaKIT Team
		*/
		struct AugmentationConfig {
			AugmentationConfig() : m_lrFlip(false), m_udFlip(false), m_rotation90(false), m_contrast(false), m_brightness(false),
				m_crop(false), m_sharpen(false), m_blur(false), m_colorwise(false), m_gradation(false),
				m_distortion(false), m_rotation(false), m_noise(false),
				m_brightnessMin(-3), m_brightnessMax(+3),
				m_sharpenKernelMin(0.1), m_sharpenKernelMax(0.8),
				m_gaussianBlurSigmaMin(0.4), m_gaussianBlurSigmaMax(1.5)

			{};
			/**
			* @brief This is a constructor of the AugmentationConfig structure.
			* @param lrFlip Input parameter for left/right flip data augmentation option.
			* @param udFlip Input parameter for up/down flip data augmentation option.
			* @param rotation90 Input parameter for rotate 90 degree data augmentation option.
			* @param contrast Input parameter for contrast variation data augmentation option.
			* @param brightness Input parameter for brightness data augmentation option.
			* @param crop Input parameter for cropping data augmentation option.
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
			*/
			AugmentationConfig(
				bool lrFlip, bool udFlip, bool rotation90, bool contrast, bool brightness,
				bool crop, bool sharpen, bool blur, bool colorwise, bool gradation,
				bool distortion, bool rotation, bool noise, int brightnessMin = -3, int brightnessMax = 3, double sharpenKernelMin = 0.1, double sharpenKernelMax = 0.8,
				double gaussianBlurKernelMin = 0.4, double gaussianBlurKernelMax = 1.5)
				: m_lrFlip(lrFlip), m_udFlip(udFlip), m_rotation90(rotation90), m_contrast(contrast), m_brightness(brightness),
				m_crop(crop), m_sharpen(sharpen), m_blur(blur), m_colorwise(colorwise), m_gradation(gradation),
				m_distortion(distortion), m_rotation(rotation), m_noise(noise),
				m_brightnessMin(brightnessMin), m_brightnessMax(brightnessMax),
				m_sharpenKernelMin(sharpenKernelMin), m_sharpenKernelMax(sharpenKernelMax),
				m_gaussianBlurSigmaMin(gaussianBlurKernelMin), m_gaussianBlurSigmaMax(gaussianBlurKernelMax)

			{};
			bool m_lrFlip, m_udFlip, m_rotation90, m_contrast, m_brightness, m_crop, m_sharpen, m_blur;
			bool m_colorwise, m_gradation, m_distortion, m_rotation, m_noise, m_reserve1, m_reserve2, m_reserve3;
			int m_brightnessMin, m_brightnessMax;
			double m_sharpenKernelMin, m_sharpenKernelMax;
			double m_gaussianBlurSigmaMin, m_gaussianBlurSigmaMax;
		};

		/**
		* @brief This is a configuration of the ClassificationTrainer.
		* @details It is used as classification train setting parameter for ClassificationTrainer.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API ClassificationTrainConfig {
		public:
			/**
			* @brief This is a constructor of ClassificationTrainConfig.
			* @param numberOfClasses The number of classes to train.
			* @param validationRatio Ratio of validation set. If user does not provide validation set, validation set is split from train set by this ratio.
			This ratio is calculated as ((validationRatio/100)*100)% of train set.
			This argument is meaningless when the user selects and uses a validation set as a separate ClassificationTrainData object.
			* @param epochs In training neural network, one epoch means one sweep of the full training set. This is converted to an iteration by the number of train sets.
			* @param modelCapacity The base (pre-defined) model capacity for executing ClassificationTrainer. {0, 1, 2, 3}.
			- 0 means small model capacity.
			- 1 means normal model capacity.
			- 2 means large model capacity.
			- 3 means extra large model capacity.
			* @param dataRatio This is a weight parameter for each class. If you want to give a larger weight to a specific class, you can increase this array's value corresponding index to the location of the specific class.
			This value will be applied at batch-sampling-time. This value affects the ratio between each class sample in training batch. 
			* @param imageHeight Height of image for train. If the learning image size is variable, set it to the largest height value in the training image.
			* @param imageWidth Width of images for train. If the learning image size is variable, set it to the largest width value in the training image.
			* @param imageChannel The number of channels of images for train.
			* @param augmentationConfig Augmentation parameters for train.
			* @param inputDataType This parameter is used to configure input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			In case of multi-image classification, we provide only 1 modelCapacity: {NORMAL}. User needs to select modelCapacity parameter NORMAL capacity.
			* @param multiImgCount The number of images for classification trainer, depends on inputDataType parameter.
			- In case of InputDataType::SINGLE, multiImgCount parameter needs to be initialized with 1.
			- In case of InputDataType::COMPARISON, multiImgCount parameter needs to be initialized with 2.
			- In case of InputDataType::MULTIMG, multiImgCount parameter needs to be initialized with multiImgCount >= 2.
			* @param imageDepth Depth of images for train.
			* @param numModelSelection Save the top N (numModelSelection) models based on the best loss.
			If the number of generated models is less than this value, only the currently created models are saved.
			* @param minimumEpoch The minimum number of epochs to save the model.
			* @param patience The number of epochs to wait before early stop if no progress on the validation set. The patience is often set somewhere between 10 and 100 (10 or 20 is more common), but it really depends on your dataset.
			- In case of ClassificationTrainer, the learning rate decay is performed once.
			*/
			__stdcall ClassificationTrainConfig(
				SuaKIT_Int32 numberOfClasses,
				SuaKIT_Int32 validationRatio,
				SuaKIT_Int32 epochs,
				SuaKIT_Int32 modelCapacity,
				const Int32Array& dataRatio,
				SuaKIT_Int32 imageHeight,
				SuaKIT_Int32 imageWidth,
				SuaKIT_Int32 imageChannel,
				const AugmentationConfig& augmentationConfig = AugmentationConfig(),
				InputDataType inputDataType = InputDataType::SINGLE,
				SuaKIT_Int32 multiImgCount = 1,
				DepthType imageDepth = DepthType::_8U,
				SuaKIT_Int32 numModelSelection = 1,
				float minimumEpoch = 0.0f,
				SuaKIT_Int64 patience = 9223372036854775807ll
			);
			__stdcall ~ClassificationTrainConfig();
			void* _GetInternal() const { return internal; }

			Status __stdcall SetClassName(SuaKIT_Int32 classIdx, const char * className);

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
		private:
			ClassificationTrainConfig(const ClassificationTrainConfig& src); //=delete;
			ClassificationTrainConfig& operator=(const ClassificationTrainConfig& rhs); //=delete;
			void *internal;
			Status m_status;
		};
		
		/**
		* @brief This is a configuration of the SegmentationTrainer.
		* @details It is used as segmentation train setting parameter for SegmentationTrainer.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API SegmentationTrainConfig {
		public:
			/**
			* @brief This is a constructor of the SegmentationTrainConfig.
			* @param numberOfClasses The number of classes to train. In segmentation training case, this parameter should be calculated except unlabeled class.
			* @param validationRatio Ratio of validation set. If user does not provide validation set, validation set is split from train set by this ratio.
			This ratio is calculated as ((validationRatio/100)*100)% of train set.
			* @param epochs In training neural network, one epoch means one sweep of the full training set. This is converted to an iteration by the number of train sets.
			* @param modelCapacity The base (pre-defined) model capacity for executing SegmentationTrainer. {0, 1, 2, 3}.
			- 0 means small model capacity.
			- 1 means normal model capacity.
			- 2 means large model capacity.
			- 3 means extra large model capacity.
			* @param dataRatio This is a weight parameter for each class. If you want to give a larger weight to specific class, you can increase this array's value corresponding index to the location of the specific class.
			This value will be applied at batch-sampling-time. This value affects the ratio between each class sample in training batch.
			* @param unlabeledRatio If you also want to adjust ratio of unlabeled patch, you can append unlabeledRatio to original dataRatio by this parameter.
			* @param strideRatio Training patch data is sampled from original image by sliding windows manner. strideRatio value means stride of shifting. 
			If you assign 50 into this parameter, then 50% size corresponding to patch size (patchSize * 0.5) would be shifting scale.
			Internally, this value must be in the range [10, 100].
			* @param patchSize The size of training patch data. This value should be multiple of 4, and must be in the range [128, max].
			You can set arbitrary value satisfying those condition, but WE HIGHLY RECOMMEND 128 or 256 for this parameter.
			* @param imageChannel The number of channels of images for train.
			* @param augmentationConfig Augmentation parameter for train.
			* @param inputDataType This parameter is used to configure input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			In case of multi-image segmentation, we provide only 2 modelCapacity: {NORMAL, LARGE}. User needs to select modelCapacity parameter NORMAL or LARGE capacity.
			* @param multiImgCount The number of images for segmentation trainer, depends on inputDataType parameter.
			- In case of InputDataType::SINGLE, multiImgCount parameter needs to be initialized with 1.
			- In case of InputDataType::COMPARISON, multiImgCount parameter needs to be initialized with 2.
			- In case of InputDataType::MULTIMG, multiImgCount parameter needs to be initialized with multiImgCount >= 2.
			* @param imageDepth Depth of images for train.
			* @param numModelSelection Save the top N (numModelSelection) models based on the best loss.
			If the number of generated models is less than this value, only the currently created models are saved.
			* @param minimumEpoch The minimum number of epochs to save the model.
			* @param patience The number of epochs to wait before early stop if no progress on the validation set. The patience is often set somewhere between 10 and 100 (10 or 20 is more common), but it really depends on your dataset.
			- In case of SegmentationTrainer, the learning rate decay is performed twice.
			*/
			__stdcall SegmentationTrainConfig(
				SuaKIT_Int32 numberOfClasses,
				SuaKIT_Int32 validationRatio,
				SuaKIT_Int32 epochs,
				SuaKIT_Int32 modelCapacity,
				const Int32Array& dataRatio,
				SuaKIT_Int32 unlabeledRatio,
				SuaKIT_Int32 strideRatio,
				SuaKIT_Int32 patchSize,
				SuaKIT_Int32 imageChannel,
				const AugmentationConfig& augmentationConfig = AugmentationConfig(),
				InputDataType inputDataType = InputDataType::SINGLE,
				SuaKIT_Int32 multiImgCount = 1,
				DepthType imageDepth = DepthType::_8U,
				SuaKIT_Int32 numModelSelection = 1,
				float minimumEpoch = 0.0f,
				SuaKIT_Int64 patience = 9223372036854775807ll
			);
			__stdcall ~SegmentationTrainConfig();
			void* _GetInternal() const { return internal; }

			Status __stdcall SetClassName(SuaKIT_Int32 classIdx, const char * className);

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
		private:
			SegmentationTrainConfig(const SegmentationTrainConfig& src); //=delete;
			SegmentationTrainConfig& operator=(const SegmentationTrainConfig& rhs); //=delete;
			void *internal;
			Status m_status;
		};

		/**
		* @brief This is a configuration of the OneClassSegmentationTrainer.
		* @details It is used as OneClassSegmentation train setting parameter for OneClassSegmentationTrainer.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API OneClassSegmentationTrainConfig {
		public:
			/**
			* @brief This is a constructor of the OneClassSegmentationTrainConfig.
			* @param validationRatio Ratio of validation set. If user does not provide validation set, validation set is split from train set by this ratio.
			This ratio is calculated as ((validationRatio/100)*100)% of train set.
			* @param epochs In training neural network, one epoch means one sweep of the full training set. This is converted to an iteration by the number of train sets.
			* @param modelCapacity Internal model capacity for executing OneClassSegmentationTrainer. {1, 2}.
			- 1 means normal model capacity.
			- 2 means large model capacity.
			In case of one class segmentation, we provide only 2 modelCapacity: {NORMAL, LARGE}. User needs to select modelCapacity parameter NORMAL or LARGE capacity.
			* @param strideRatio Training patch data is sampled from original image by sliding windows manner. strideRatio value means stride of shifting.
			If you assign 50 into this parameter, then 50% size corresponding to patch size (patchSize * 0.5) would be shifting scale.
			Internally, this value must be in the range [10, 100].
			* @param patchSize The size of training patch data. This value should be multiple of 4, and must be in the range [64, max].
			You can set arbitrary value satisfying those condition, but WE HIGHLY RECOMMEND 64 or 128 or 256 for this parameter.
			* @param imageChannel The number of channels of images for train.
			* @param augmentationConfig Augmentation parameter for train.
			Currently, augmentation for OneClassSegmentation is not supported. Just put default constructor.
			* @param inputDataType This parameter is used to configure input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			Currently, OneClassSegmentation only support SINGLE type.
			* @param multiImgCount The number of images for segmentation trainer, depends on inputDataType parameter.
			- In case of InputDataType::SINGLE, multiImgCount parameter needs to be initialized with 1.
			- In case of InputDataType::COMPARISON, multiImgCount parameter needs to be initialized with 2.
			- In case of InputDataType::MULTIMG, multiImgCount parameter needs to be initialized with multiImgCount >= 2.
			Currently, OneClassSegmentation only support SINGLE type. (multiImgCount should be 1)
			* @param imageDepth Depth of images for train.
			* @param numModelSelection Save the top N (numModelSelection) models based on the best loss.
			If the number of generated models is less than this value, only the currently created models are saved.
			* @param minimumEpoch The minimum number of epochs to save the model.
			* @param patience The number of epochs to wait before early stop if no progress on the validation set. The patience is often set somewhere between 10 and 100 (10 or 20 is more common), but it really depends on your dataset.
			- In case of OneClassSegmentationTrainer, the learning rate decay is not performed.
			*/
			__stdcall OneClassSegmentationTrainConfig(
				SuaKIT_Int32 validationRatio,
				SuaKIT_Int32 epochs,
				SuaKIT_Int32 modelCapacity,
				SuaKIT_Int32 strideRatio,
				SuaKIT_Int32 patchSize,
				SuaKIT_Int32 imageChannel,
				const AugmentationConfig& augmentationConfig = AugmentationConfig(),
				InputDataType inputDataType = InputDataType::SINGLE,
				SuaKIT_Int32 multiImgCount = 1,
				DepthType imageDepth = DepthType::_8U,
				SuaKIT_Int32 numModelSelection = 1,
				float minimumEpoch = 0.0f,
				SuaKIT_Int64 patience = 9223372036854775807ll
			);
			__stdcall ~OneClassSegmentationTrainConfig();
			void* _GetInternal() const { return internal; }

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
		private:
			OneClassSegmentationTrainConfig(const OneClassSegmentationTrainConfig& src); //=delete;
			OneClassSegmentationTrainConfig& operator=(const OneClassSegmentationTrainConfig& rhs); //=delete;
			void *internal;
			Status m_status;
		};
		
		/**
		* @brief This is a configuration of the DetectionTrainer.
		* @details It is used as detection train setting parameter for DetectionTrainer.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API DetectionTrainConfig {
		public:
			/**
			* @brief This is a constructor of the DetectionTrainConfig.
			* @param numberOfClasses The number of classes to train. In detection training case, this parameter should be calculated except unlabeled class.
			* @param validationRatio Ratio of validation set. If user does not provide validation set, validation set is split from train set by this ratio.
			This ratio is calculated as ((validationRatio/100)*100)% of train set.
			* @param epochs In training neural network, one epoch means one sweep of the full training set. This is converted to an iteration according to the number of train sets.
			* @param modelCapacity The base (pre-defined) model capacity for executing DetectionTrainer. {0, 1, 2, 3}.
			- 0 means small model capacity.
			- 1 means normal model capacity.
			- 2 means large model capacity.
			- 3 means extra large model capacity.
			* @param dataRatio This is a weight parameter for each class. If you want to give a larger weight to specific class, you can increase this array's value corresponding index to the location of the specific class.
			* @param imageHeight Height of image for train. If the learning image size is variable, set it to the largest height value in the training image.
			* @param imageWidth Width of images for train. If the learning image size is variable, set it to the largest width value in the training image.
			* @param imageChannel The number of channels of images for train.
			* @param anchors Size of anchor boxes. 
			Actual shape : (2, N) s.t. N = number of anchor box
			if you want to apply 3 anchor boxes, then Actual shape will be = (2, 3) and N = 3
			and configuration of array would be { width1, width2, width3, height1, height2, height3 }
			Value range : [0, 1]  => (normalized to image size)
			* @param augmentationConfig Augmentation parameters for train.
			* @param inputDataType This parameter is used to configure input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			InputDataType::COMPARISON and InputDataType::MULTIMG are currently not supported. These features will be updated later.
			* @param multiImgCount The number of images for detection trainer, depends on inputDataType parameter.
			- In case of InputDataType::SINGLE, multiImgCount parameter needs to be initialized with 1.
			- In case of InputDataType::COMPARISON, multiImgCount parameter needs to be initialized with 2.
			- In case of InputDataType::MULTIMG, multiImgCount parameter needs to be initialized with multiImgCount >= 2.
			* @param imageDepth Depth of images for train.
			* @param detectionModelType This parameter is used to configure model type of detection. Set it to one of {ModelType::DETECTION_DOLPHIN, ModelType::DETECTION_DRAGON}.
			* @param numModelSelection Save the top N (numModelSelection) models based on the best loss.
			If the number of generated models is less than this value, only the currently created models are saved.
			* @param minimumEpoch The minimum number of epochs to save the model.
			* @param patience The number of epochs to wait before early stop if no progress on the validation set. The patience is often set somewhere between 10 and 100 (10 or 20 is more common), but it really depends on your dataset.
			- In case of DetectionTrainer, the learning rate decay is not performed.
			*/
			__stdcall DetectionTrainConfig(
				SuaKIT_Int32 numberOfClasses,
				SuaKIT_Int32 validationRatio,
				SuaKIT_Int32 epochs,
				SuaKIT_Int32 modelCapacity,
				const Int32Array& dataRatio,
				SuaKIT_Int32 imageHeight,
				SuaKIT_Int32 imageWidth,
				SuaKIT_Int32 imageChannel,
				const FloatArray& anchors,
				const AugmentationConfig& augmentationConfig = AugmentationConfig(),
				InputDataType inputDataType = InputDataType::SINGLE,
				SuaKIT_Int32 multiImgCount = 1,
				DepthType imageDepth = DepthType::_8U,
				ModelType detectionModelType = ModelType::DETECTION_DOLPHIN,
				SuaKIT_Int32 numModelSelection = 1,
				float minimumEpoch = 0.0f,
				SuaKIT_Int64 patience = 9223372036854775807ll
			);
			__stdcall ~DetectionTrainConfig();
			void* _GetInternal() const { return internal; }

			Status SetClassName(SuaKIT_Int32 classIdx, const char * className);

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
		private:
			DetectionTrainConfig(const DetectionTrainConfig& src); //=delete;
			DetectionTrainConfig& operator=(const DetectionTrainConfig& rhs); //=delete;
			void *internal;
			Status m_status;
		};

		/**
		* @brief This class is used to manage data for classification train.
		* @details This class contains image and label information for training ClassificationTrainer.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API ClassificationTrainData {
		public:
			/**
			* @brief A constructor of ClassificationTrainData.
			* @param inputDataType This parameter is used to configure input type of neural network. It initializes {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			*/
			__stdcall ClassificationTrainData(InputDataType inputDataType = SINGLE);
			__stdcall ~ClassificationTrainData();
			/**
			* @brief Push single data for train ClassificationTrainer. It should be used for InputDataType::SINGLE only.
			* @param imgPath A path of image to be trained.
			* @param classNum The class identifier for the image. If the total number of classes is (N), this parameter should be a value less than N (classNum <N).
			* @param maskImagePath A path of mask image. The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masked and 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushSingleDataInfo(const wchar_t* imgPath, SuaKIT_Int32 classNum, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());
			/**
			* @brief Push comparison data for train ClassificationTrainer. It should be used for InputDataType::COMPARISON only.
			* @param masterImagePath A path of master image to be trained.
			* @param slaveImagePath A path of slave image to be trained.
			* @param classNum The class identifier for the image. If the total number of classes is (N), this parameter should be a value less than N (classNum <N).
			* @param maskImagePath Path of mask image. The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masked and 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushComparsionDataInfo(const wchar_t* masterImagePath, const wchar_t* slaveImagePath, SuaKIT_Int32 classNum, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());
			/**
			* @brief Push single data for train ClassificationTrainer. It should be used for InputDataType::MULTIMG only.
			* @param imagePaths Paths of images to be trained. It takes the path of images as a string separated by newline characters ('\n'). For example, the input format should be "C:\Folder\img1.png\nC:\Folder\img2.png\nC:\Folder\img3.png" and the order of the images is important.
			* @param classNum The class identifier for the image. If the total number of classes is (N), this parameter should be a value less than N (classNum <N).
			* @param maskImagePath A path of mask image. The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masked and 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushMultiDataInfo(const wchar_t* imagePaths, SuaKIT_Int32 classNum, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());
			
			Status __stdcall PushSingleDataInfoWithJson(const wchar_t* imgPath, SuaKIT_Int32 classNum, const wchar_t* maskImageJson, const Rect& roi = Rect());
			Status __stdcall PushComparsionDataInfoWithJson(const wchar_t* masterImagePath, const wchar_t* slaveImagePath, SuaKIT_Int32 classNum, const wchar_t* maskImageJson, const Rect& roi = Rect());
			Status __stdcall PushMultiDataInfoWithJson(const wchar_t* imagePaths, SuaKIT_Int32 classNum, const wchar_t* maskImageJson, const Rect& roi = Rect());

			/**
			* @brief Get current ClassificationTrainData's InputDataType.
			* @return Returns InputDataType.
			*/
			InputDataType __stdcall GetDataType() const;
			/**
			* @brief Get length of current ClassificationTrainData.
			* @return Returns the length of image data as SuaKIT_Int64 type.
			*/
			SuaKIT_Int64 __stdcall GetTrainDataLength() const;

			void* _getInternal() const { return internal; }

			/**
			* @brief Explicit resource release. It is useful in non-native languages (such as C#).
			* @return Returns Status.
			*/
			Status __stdcall Destroy();

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
		private:
			ClassificationTrainData(const ClassificationTrainData& src); //=delete;
			ClassificationTrainData& operator=(const ClassificationTrainData& rhs); //=delete;
			void* internal;
			InputDataType m_dataType;
			Status m_status;
		};

		/**
		* @brief This class is used to manage data for segmentation train.
		* @details This class contains image and label information for training SegmentationTrainer.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API SegmentationTrainData {
		public:
			/**
			* @brief A constructor of SegmentationTrainData.
			* @param inputDataType This parameter used for configurate input type of neural network. It initializes {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			*/
			__stdcall SegmentationTrainData(InputDataType inputDataType = SINGLE);
			__stdcall ~SegmentationTrainData();
			/**
			* @brief Push single data for train SegmentationTrainer. It should be used for InputDataType::SINGLE only.
			* @param imgPath A path of image to be trained.
			* @param labelImgPath A path of label image. The label image is an 8-bit 1-channel image, with each pixel value representing a class number and an unlabeled area having an 255 pixel value.
			* @param maskImagePath A path of mask image. The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masking. 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushSingleDataInfo(const wchar_t* imgPath, const wchar_t* labelImgPath, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());
			/**
			* @brief Push comparison data for train SegmentationTrainer. It should be used for InputDataType::COMPARISON only.
			* @param masterImagePath A path of master image to be trained.
			* @param slaveImagePath A path of slave image to be trained.
			* @param labelImgPath A paThe label image is an 8-bit 1-channel image, with each pixel value representing a class number and an unlabel area having an 255 pixel value.
			* @param maskImagePath Path of mask image. The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masking. 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushComparsionDataInfo(const wchar_t* masterImagePath, const wchar_t* slaveImagePath, const wchar_t* labelImgPath, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());
			/**
			* @brief Push single data for train SegmentationTrainer. It should be used for InputDataType::MULTIMG only.
			* @param imagePaths Paths of images to be trained. It takes the path of images as a string separated by newline characters ('\n'). For example, the input format should be "C:\Folder\img1.png\nC:\Folder\img2.png\nC:\Folder\img3.png" and the order of the images is important.
			* @param labelImgPath A path of label image. The label image is an 8-bit 1-channel image, with each pixel value representing a class number and an unlabel area having an 255 pixel value.
			* @param maskImagePath A path of mask image. The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masked and 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushMultiDataInfo(const wchar_t* imagePaths, const wchar_t* labelImgPath, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());
			
			Status __stdcall PushSingleDataInfoWithJson(const wchar_t* imgPath, const wchar_t* labelJson, const wchar_t* maskImageJson, const Rect& roi = Rect());
			Status __stdcall PushComparsionDataInfoWithJson(const wchar_t* masterImagePath, const wchar_t* slaveImagePath, const wchar_t* labelJson, const wchar_t* maskImageJson, const Rect& roi = Rect());
			Status __stdcall PushMultiDataInfoWithJson(const wchar_t* imagePaths, const wchar_t* labelJson, const wchar_t* maskImageJson, const Rect& roi = Rect());

			/**
			* @brief Get current SegmentationTrainData's InputDataType.
			* @return Returns InputDataType.
			*/
			InputDataType __stdcall GetDataType() const;
			/**
			* @brief Get length of current SegmentationTrainData.
			* @return Returns the length of image data as SuaKIT_Int64 type.
			*/
			SuaKIT_Int64 __stdcall GetTrainDataLength() const;

			void* _getInternal() const { return internal; }

			/**
			* @brief Explicit resource release. It is useful in non-native languages (such as C#).
			* @return Returns Status.
			*/
			Status __stdcall Destroy();

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
		private:
			SegmentationTrainData(const SegmentationTrainData& src); //=delete;
			SegmentationTrainData& operator=(const SegmentationTrainData& rhs); //=delete;
			void* internal;
			InputDataType m_dataType;
			Status m_status;
		};

		/**
		* @brief This class is used to manage data for OneClassSegmentation train.
		* @details This class contains image information for training OneClassSegmentationTrainer.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API OneClassSegmentationTrainData {
		public:
			/**
			* @brief A constructor of OneClassSegmentationTrainData.
			* @param inputDataType This parameter used for configurate input type of neural network. It initializes {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			Currently, OneClassSegmentation only support InputDataType::SINGLE.
			*/
			__stdcall OneClassSegmentationTrainData(InputDataType inputDataType = SINGLE);
			__stdcall ~OneClassSegmentationTrainData();
			/**
			* @brief Push single data for train OneClassSegmentationTrainer. It should be used for InputDataType::SINGLE only.
			* @param imgPath A path of image to be trained.
			* @param maskImagePath A path of mask image. The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masking. 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushSingleDataInfo(const wchar_t* imgPath, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());
			/**
			* @brief Push comparison data for train OneClassSegmentationTrainer. It should be used for InputDataType::COMPARISON only.
			This function is not supported currently.
			* @param masterImagePath A path of master image to be trained.
			* @param slaveImagePath A path of slave image to be trained.
			* @param maskImagePath Path of mask image. The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masking. 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushComparsionDataInfo(const wchar_t* masterImagePath, const wchar_t* slaveImagePath, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());
			/**
			* @brief InputDataType::MULTIMG is currently not supported.
			Push single data for train OneClassSegmentationTrainer. It should be used for InputDataType::MULTIMG only.
			* @param imagePaths Paths of images to be trained. It takes the path of images as a string separated by newline characters ('\n'). For example, the input format should be "C:\Folder\img1.png\nC:\Folder\img2.png\nC:\Folder\img3.png" and the order of the images is important.
			* @param maskImagePath A path of mask image. The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masked and 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushMultiDataInfo(const wchar_t* imagePaths, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());

			Status __stdcall PushSingleDataInfoWithJson(const wchar_t* imgPath, const wchar_t* maskImageJson, const Rect& roi = Rect());
			Status __stdcall PushComparsionDataInfoWithJson(const wchar_t* masterImagePath, const wchar_t* slaveImagePath, const wchar_t* maskImageJson, const Rect& roi = Rect());
			Status __stdcall PushMultiDataInfoWithJson(const wchar_t* imagePaths, const wchar_t* maskImageJson, const Rect& roi = Rect());

			/**
			* @brief Get current OneClassSegmentationTrainData's InputDataType.
			* @return Returns InputDataType.
			*/
			InputDataType __stdcall GetDataType() const;
			/**
			* @brief Get length of current OneClassSegmentationTrainData.
			* @return Returns the length of image data as SuaKIT_Int64 type.
			*/
			SuaKIT_Int64 __stdcall GetTrainDataLength() const;

			void* _getInternal() const { return internal; }

			/**
			* @brief Explicit resource release. It is useful in non-native languages (such as C#).
			* @return Returns Status.
			*/
			Status __stdcall Destroy();

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
		private:
			OneClassSegmentationTrainData(const OneClassSegmentationTrainData& src); //=delete;
			OneClassSegmentationTrainData& operator=(const OneClassSegmentationTrainData& rhs); //=delete;
			void* internal;
			InputDataType m_dataType;
			Status m_status;
		};

		/**
		* @brief This class is used to manage data for detection train.
		* @details This class contains image and label information for training DetectionTrainer.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API DetectionTrainData {
		public:
			/**
			* @brief A constructor of DetectionTrainData.
			* @param inputDataType This parameter is used to configure input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			InputDataType::COMPARISON and InputDataType::MULTIMG are currently not supported. These features will be updated later.
			*/
			__stdcall DetectionTrainData(InputDataType inputDataType = SINGLE);
			__stdcall ~DetectionTrainData();

			/**
			* @brief Push single data for train DetectionTrainer. It should be used for InputDataType::SINGLE only.
			* @param imgPath A path of image to be trained.
			* @param labelRects The labelRects is an array of rectangle information, which include rect coordinate (x, y, width, height) and class number corresponding to each object.
			* @param maskImagePath A path of mask image. The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masked and 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushSingleDataInfo(const wchar_t* imgPath, const RectArray& labelRects, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());
			/**
			* @brief InputDataType::COMPARISON is currently not supported. This feature will be updated later.
			Push comparison data for train DetectionTrainer. It should be used for InputDataType::COMPARISON only.
			* @param masterImagePath A path of master image to be trained.
			* @param slaveImagePath A path of replica image to be trained.
			* @param labelRects The labelRects is an array of rectangle information, which include rect coordinate (x, y, width, height) and class number corresponding to each object.
			* @param maskImagePath A path of mask image.The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masked and 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushComparsionDataInfo(const wchar_t* masterImagePath, const wchar_t* slaveImagePath, const RectArray& labelRects, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());
			/**
			* @brief InputDataType::MULTIMG is currently not supported. This feature will be updated later.
			Push single data for train DetectionTrainer. It should be used for InputDataType::MULTIMG only.
			* @param imagePaths Paths of images to be trained. It takes the path of images as a string separated by newline characters ('\n'). For example, the input format should be "C:\Folder\img1.png\nC:\Folder\img2.png\nC:\Folder\img3.png" and the order of the images is important.
			* @param labelRects The labelRects is an array of rectangle information, which include rect coordinate (x, y, width, height) and class number corresponding to each object.
			* @param maskImagePath A path of mask image. The mask image must be a 1-channel image and needs to have a value 0 or 255, where 0 indicates masking. 255 means not masked.
			* @param roi ROI(region of interest) rect.
			* @return Returns Status.
			*/
			Status __stdcall PushMultiDataInfo(const wchar_t* imagePaths, const RectArray& labelRects, const wchar_t* maskImagePath = 0, const Rect& roi = Rect());

			Status __stdcall PushSingleDataInfoWithJson(const wchar_t* imgPath, const RectArray& labelRects, const wchar_t* maskImageJson, const Rect& roi = Rect());
			Status __stdcall PushComparsionDataInfoWithJson(const wchar_t* masterImagePath, const wchar_t* slaveImagePath, const RectArray& labelRects, const wchar_t* maskImageJson, const Rect& roi = Rect());
			Status __stdcall PushMultiDataInfoWithJson(const wchar_t* imagePaths, const RectArray& labelRects, const wchar_t* maskImageJson, const Rect& roi = Rect());

			/**
			* @brief Get current DetectionTrainData's InputDataType.
			* @return Returns InputDataType.
			*/
			InputDataType __stdcall GetDataType() const;
			/**
			* @brief Get length of current DetectionTrainData.
			* @return Returns the length of image data as SuaKIT_Int64 type.
			*/
			SuaKIT_Int64 __stdcall GetTrainDataLength() const;

			void* _getInternal() const { return internal; }

			/**
			* @brief Explicit resource release. It is useful in non-native languages (such as C#).
			* @return Returns Status.
			*/
			Status __stdcall Destroy();

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
		private:
			DetectionTrainData(const DetectionTrainData& src); //=delete;
			DetectionTrainData& operator=(const DetectionTrainData& rhs); //=delete;
			void* internal;
			InputDataType m_dataType;
			Status m_status;
		};

		/**
		* @brief This class is used at classification training time.
		* @details This class can produce a new artificial neural network that can solve classification problem. The user must prepare a ClassificationTrainConfig object and a ClassificationTrainData object.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API ClassificationTrainer {
		public:
			/**
			* @brief A constructor of ClassificationTrainer.
			* @param inputDataType This parameter used for configurate input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			* @param devDesc Specifies the device on which the ClassificationTrainer will operate. ClassificationTrainer can operate on one device and support CPU and GPU.
			* @param traincfg Configuration of ClassificationTrainer.
			* @param modelSavePath After the train is finished, the ClassificationTrainer will save the best model to disk and specify the path to the best model to be stored. The best model is saved under the path specified by modelSavePath.
			* @param baseModelPath The base (pre-defined) model path for executing ClassificationTrainer. 
			As using backslash(`\`) characters in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			For example, `L"C:/suakit/weights/small"` is preferred over `L"C:\\suakit\\weights\\small"`.
			There is a file named {small, normal, large, extra_large} inside the weights folder in the path where the suakit is installed.
			This is the structure of the pre-defined model saved as a file.
			- small refers to the small model.
			- normal refers to the normal model.
			- large refers to the large model.
			- extra_large refers to the extra large model.
        	You can also set this value to trained model path for continue training mode. (Currently, single classification continue training mode is supported only)
			* @return Returns Status.
			*/
			__stdcall ClassificationTrainer(InputDataType inputDataType, const DeviceDescriptor& devDesc, const ClassificationTrainConfig& traincfg, const wchar_t * modelSavePath, const wchar_t * baseModelPath = 0); // baseModelPath  = pretrained model
			__stdcall ~ClassificationTrainer();

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
			/**
			* @brief Get current ClassificationTrainer's InputDataType.
			* @return Returns InputDataType.
			*/
			InputDataType __stdcall GetDataType() const;

			/**
			* @brief During ClassificationTrainer object is training, this function returns true. Else return false.
			* @return Returns boolean parameter represents currently training status.
			*/
			bool __stdcall isTraining() const;
			/**
			* @brief Trainer starts to train artificial neural network by calling this function.
			* @details This function takes just one argument. ClassificationTrainData. 
			It works asynchronously and terminates the function almost simultaneously with function execution.
			This function internally creates a thread to train, so you do not need to write an explicit thread wrapper when you want to do something else.
			When users use this function, ClassificationTrainer will use a certain percentage of the training set as a validation set.
			* @param trainData ClassificationTrainData to train. See ClassificationTrainData for details.
			* @return Returns Status.
			*/
			Status __stdcall StartTrain(const ClassificationTrainData& trainData);
			/**
			* @brief Trainer starts to train artificial neural network by calling this function.
			* @details This function takes just two arguments. They are both ClassificationTrainData. One is for train data, and the other is for validation data.
			It works asynchronously and terminates the function almost simultaneously with function execution.
			This function internally creates a thread to train, so you do not need to write an explicit thread wrapper when you want to do something else.
			This function explicitly sets the validation set and proceeds with the learning.
			* @param trainData ClassificationTrainData to train. See ClassificationTrainData for details.
			* @param validationData Learn by specifying explicit validation data.
			* @return Returns Status.
			*/
			Status __stdcall StartTrain(const ClassificationTrainData& trainData, const ClassificationTrainData& validationData);

			/**
			* @brief Blocking function until the train is finished.
			* @details Calling this function will block until the train is terminated.
			* @return Returns Status.
			*/
			Status __stdcall WaitTrain();
			
			/**
			* @brief Returns a Message representing the current training state.
			* @details ClassificationTrainer pushes its message into an internally managed message queue and proceeds with learning as it progresses.
			When the GetMessage function is called, the internally managed message in the queue is dequeued and returned to the user as a Message object.
			If there is no message in the internal message queue, it will be blocked until a message is enqueued. If a message is enqueued, the message will be dequeued and returned immediately.
			* @return Returns Message object.
			*/
			Message __stdcall GetMessage();

			/**
			* @brief Returns internal Message queue is empty
			* @details ClassificationTrainer has message queue for communication. This function returns true when that queue is empty, otherwise return false.
			* @return is message queue is empty
			*/
			bool __stdcall MessageIsEmpty();

			/**
			* @brief If training is currently in progress, we stop training. If the training is not in progress, nothing happens on the call.
			* @return Returns Status.
			*/
			Status __stdcall StopTrain();
			
			/**
			* @brief Explicit resource release. It is useful in non-native languages (such as C#).

			* @return Returns Status.
			*/
			Status __stdcall Destroy();

			/**
			* @brief Returns the maximum Image count that can be used when training a multi-image type model.
			multiImgCount in ClassificationTrainConfig constructor argument, must not exceed this return value.
			* @details imageHeight Maximum height of image to use for training
			* @details imageWidth Maximum width of image to use for training
			* @details imageChannel channel of image to use for training
			* @return Returns he maximum Image count
			*/
			static SuaKIT_Int64 GetMaxImageCount(
				SuaKIT_Int32 imageHeight,
				SuaKIT_Int32 imageWidth,
				SuaKIT_Int32 imageChannel
			);
		private:
			ClassificationTrainer(const ClassificationTrainer& src); //=delete;
			ClassificationTrainer& operator=(const ClassificationTrainer& rhs); //=delete;
			DeviceDescriptor m_devDesc;
			InputDataType m_dataType;
			Status m_status;
			void* internal;
		};

		/**
		* @brief This class is used at segmentation training time.
		* @details This class can produce a new artificial neural network that can solve segmentation problem. The user must prepare a SegmentationTrainConfig object and a SegmentationTrainData object.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API SegmentationTrainer {

		public:
			/**
			* @brief A constructor of SegmentationTrainer.
			* @param inputDataType This parameter used for configurate input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			* @param devDesc Specifies the device on which the SegmentationTrainer will operate. SegmentationTrainer can operate on one device and supports CPU and GPU.
			* @param traincfg Configuration of SegmentationTrainer.
			* @param modelSavePath After the train is finished, the SegmentationTrainer will save the best model to disk and specify the path to the best model to be stored. The best model is saved under the path specified by modelSavePath.
			* @param baseModelPath The base (pre-defined) model path for executing SegmentationTrainer. 
			As using backslash(`\`) characters in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			For example, `L"C:/suakit/weights/small"` is preferred over `L"C:\\suakit\\weights\\small"`.
			There is a file named {small, normal, large, extra_large} inside the weights folder in the path where the suakit is installed.
			This is the structure of the pre-defined model saved as a file.			
			- small refers to the small model.
			- normal refers to the normal model.
			- large refers to the large model.
			- extra_large refers to the extra large model.
        	You can also set this value to trained model path for continue training mode. (Currently, single classification continue training mode is supported only)
			* @return Returns Status.
			*/
			__stdcall SegmentationTrainer(InputDataType inputDataType, const DeviceDescriptor& devDesc, const SegmentationTrainConfig& traincfg, const wchar_t * modelSavePath, const wchar_t * baseModelPath = 0);
			__stdcall ~SegmentationTrainer();

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
			/**
			* @brief Get current SegmentationTrainer's InputDataType.
			* @return Returns InputDataType.
			*/
			InputDataType __stdcall GetDataType() const;

			/**
			* @brief During SegmentationTrainer object is training, this function returns true. Else return false.
			* @return Returns boolean parameter represents currently training status.
			*/
			bool __stdcall isTraining() const;
			/**
			* @brief Trainer starts to train artificial neural network by calling this function.
			* @details This function takes just one argument. SegmentationTrainData.
			It works asynchronously and terminates the function almost simultaneously with function execution.
			This function internally creates a thread to train, so you do not need to write an explicit thread wrapper when you want to do something else.
			When users use this function, SegmentationTrainer will use a certain percentage of the training set as a validation set.
			* @param trainData SegmentationTrainData to train. See SegmentationTrainData for details.
			* @return Returns Status.
			*/
			Status __stdcall StartTrain(const SegmentationTrainData& trainData);
			/**
			* @brief Trainer starts to train artificial neural network by calling this function.
			* @details This function takes just two arguments. They are both SegmentationTrainData. One is for train data, and the other is for validation data.
			It works asynchronously and terminates the function almost simultaneously with function execution.
			This function internally creates a thread to train, so you do not need to write an explicit thread wrapper when you want to do something else.
			This function explicitly sets the validation set and proceeds with the learning.
			* @param trainData SegmentationTrainData to train. See SegmentationTrainData for details.
			* @param validationData Learn by specifying explicit validation data.
			* @return Returns Status.
			*/
			Status __stdcall StartTrain(const SegmentationTrainData& trainData, const SegmentationTrainData& validationData);

			/**
			* @brief Blocking function until the train is finished.
			* @details Calling this function will block until the train is terminated.
			* @return Returns Status.
			*/
			Status __stdcall WaitTrain();

			/**
			* @brief Returns a Message representing the current training state.
			* @details SegmentationTrainer pushes its message into an internally managed message queue and proceeds with learning as it progresses.
			When the GetMessage function is called, the internally managed message in the queue is dequeued and returned to the user as a Message object.
			If there is no message in the internal message queue, it will be blocked until a message is enqueued. If a message is enqueued, the message will be dequeued and returned immediately.
			* @return Returns Message object.
			*/
			Message __stdcall GetMessage();

			/**
			* @brief Returns internal Message queue is empty
			* @details ClassificationTrainer has message queue for communication. This function returns true when that queue is empty, otherwise return false.
			* @return is message queue is empty
			*/
			bool __stdcall MessageIsEmpty();

			/**
			* @brief If training is currently in progress, we stop training. If the training is not in progress, nothing happens on the call.
			* @return Returns Status.
			*/
			Status __stdcall StopTrain();

			/**
			* @brief Explicit resource release. It is useful in non-native languages (such as C#).
			* @return Returns Status.
			*/
			Status __stdcall Destroy();

		private:
			SegmentationTrainer(const SegmentationTrainer& src); //=delete;
			SegmentationTrainer& operator=(const SegmentationTrainer& rhs); //=delete;
			DeviceDescriptor m_devDesc;
			InputDataType m_dataType;
			Status m_status;
			void* internal;
		};

		/**
		* @brief This class is used at OneClassSegmentation training time.
		* @details This class can produce a new artificial neural network that can solve OneClassSegmentation problem. The user must prepare a OneClassSegmentationTrainConfig object and a OneClassSegmentationTrainData object.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API OneClassSegmentationTrainer {

		public:
			/**
			* @brief A constructor of OneClassSegmentationTrainer.
			* @param inputDataType This parameter used for configurate input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			Currently, OneClassSegmentation only support InputDataType::SINGLE.
			* @param devDesc Specifies the device on which the OneClassSegmentationTrainer will operate. OneClassSegmentationTrainer can operate on one device and supports CPU and GPU.
			* @param traincfg Configuration of OneClassSegmentationTrainer.
			* @param modelSavePath After the train is finished, the OneClassSegmentationTrainer will save the best model to disk and specify the path to the best model to be stored. The best model is saved under the path specified by modelSavePath.
			* @param baseModelPath The base (pre-defined) model path for executing OneClassSegmentationTrainer.
			Currently, pre-trained base model is not supported.
        	You can also set this value to trained model path for continue training mode. (Currently, single classification continue training mode is supported only)
			* @return Returns Status.
			*/
			__stdcall OneClassSegmentationTrainer(InputDataType inputDataType, const DeviceDescriptor& devDesc, const OneClassSegmentationTrainConfig& traincfg, const wchar_t * modelSavePath, const wchar_t * baseModelPath = 0);
			__stdcall ~OneClassSegmentationTrainer();

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
			/**
			* @brief Get current OneClassSegmentationTrainer's InputDataType.
			* @return Returns InputDataType.
			*/
			InputDataType __stdcall GetDataType() const;

			/**
			* @brief During OneClassSegmentationTrainer object is training, this function returns true. Else return false.
			* @return Returns boolean parameter represents currently training status.
			*/
			bool __stdcall isTraining() const;
			/**
			* @brief Trainer starts to train artificial neural network by calling this function.
			* @details This function takes just one argument. OneClassSegmentationTrainData.
			It works asynchronously and terminates the function almost simultaneously with function execution.
			This function internally creates a thread to train, so you do not need to write an explicit thread wrapper when you want to do something else.
			When users use this function, OneClassSegmentationTrainer will use a certain percentage of the training set as a validation set.
			* @param trainData OneClassSegmentationTrainData to train. See OneClassSegmentationTrainData for details.
			* @return Returns Status.
			*/
			Status __stdcall StartTrain(const OneClassSegmentationTrainData& trainData);
			/**
			* @brief Trainer starts to train artificial neural network by calling this function.
			* @details This function takes just two arguments. They are both OneClassSegmentationTrainData. One is for train data, and the other is for validation data.
			It works asynchronously and terminates the function almost simultaneously with function execution.
			This function internally creates a thread to train, so you do not need to write an explicit thread wrapper when you want to do something else.
			This function explicitly sets the validation set and proceeds with the learning.
			* @param trainData OneClassSegmentationTrainData to train. See OneClassSegmentationTrainData for details.
			* @param validationData Learn by specifying explicit validation data.
			* @return Returns Status.
			*/
			Status __stdcall StartTrain(const OneClassSegmentationTrainData& trainData, const OneClassSegmentationTrainData& validationData);

			/**
			* @brief Blocking function until the train is finished.
			* @details Calling this function will block until the train is terminated.
			* @return Returns Status.
			*/
			Status __stdcall WaitTrain();

			/**
			* @brief Returns a Message representing the current training state.
			* @details OneClassSegmentationTrainer pushes its message into an internally managed message queue and proceeds with learning as it progresses.
			When the GetMessage function is called, the internally managed message in the queue is dequeued and returned to the user as a Message object.
			If there is no message in the internal message queue, it will be blocked until a message is enqueued. If a message is enqueued, the message will be dequeued and returned immediately.
			* @return Returns Message object.
			*/
			Message __stdcall GetMessage();

			/**
			* @brief Returns internal Message queue is empty
			* @details ClassificationTrainer has message queue for communication. This function returns true when that queue is empty, otherwise return false.
			* @return is message queue is empty
			*/
			bool __stdcall MessageIsEmpty();

			/**
			* @brief If training is currently in progress, we stop training. If the training is not in progress, nothing happens on the call.
			* @return Returns Status.
			*/
			Status __stdcall StopTrain();

			/**
			* @brief Explicit resource release. It is useful in non-native languages (such as C#).
			* @return Returns Status.
			*/
			Status __stdcall Destroy();

		private:
			OneClassSegmentationTrainer(const OneClassSegmentationTrainer& src); //=delete;
			OneClassSegmentationTrainer& operator=(const OneClassSegmentationTrainer& rhs); //=delete;
			DeviceDescriptor m_devDesc;
			InputDataType m_dataType;
			Status m_status;
			void* internal;
		};

		/**
		* @brief This class is used at detection training time.
		* @details This class can produce a new artificial neural network that can solve detection problem. The user must prepare a DetectionTrainConfig object and a DetectionTrainData object.
		* @author ?Sualab SuaKIT Team
		*/
		class SUAKIT_API DetectionTrainer {

		public:
			/**
			* @brief A constructor of DetectionTrainer.
			* @param inputDataType This parameter used for configurate input type of neural network. Set it to one of {InputDataType::SINGLE, InputDataType::COMPARISON, InputDataType::MULTIMG}.
			InputDataType::COMPARISON and InputDataType::MULTIMG are currently not supported. These features will be updated later.
			* @param devDesc Specifies the device on which the DetectionTrainer will operate. DetectionTrainer can operate on one device and supports CPU and GPU.
			* @param traincfg Configuration of DetectionTrainer.
			* @param modelSavePath After the train is finished, the DetectionTrainer will save the best model to disk and specify the path to the best model to be stored. The best model is saved under the path specified by modelSavePath.
			* @param baseModelPath The base (pre-defined) model path for executing DetectionTrainer.
			As using backslash(`\`) characters in `const wchar_t*` strings is inconvenient for a variety of reasons, we recommend using paths with forward slashes(`/`).
			For example, `L"C:/suakit/weights/small"` is preferred over `L"C:\\suakit\\weights\\small"`.
			There is a file named {small, normal, detection_large, extra_large} inside the weights folder in the path where the suakit is installed.
			This is the structure of the pre-defined model saved as a file.			
			- small refers to the small model.
			- normal refers to the normal model.
			- detection_large refers to the large model.
			- extra_large refers to the extra large model.
        	You can also set this value to trained model path for continue training mode. (Currently, single classification continue training mode is supported only)
			* @return Returns Status.
			*/
			__stdcall DetectionTrainer(InputDataType inputDataType, const DeviceDescriptor& devDesc, const DetectionTrainConfig& traincfg, const wchar_t * modelSavePath, const wchar_t * baseModelPath = 0);
			__stdcall ~DetectionTrainer();

			/**
			* @return Returns Status.
			*/
			Status __stdcall GetStatus() const;
			/**
			* @brief Get current DetectionTrainer's InputDataType.
			* @return Returns InputDataType.
			*/
			InputDataType __stdcall GetDataType() const;

			/**
			* @brief During DetectionTrainer object is training, this function returns true. Else return false.
			* @return Returns boolean parameter represents currently training status.
			*/
			bool __stdcall isTraining() const;
			/**
			* @brief Trainer starts to train artificial neural network by calling this function.
			* @details This function takes just one argument. DetectionTrainData.
			It works asynchronously and terminates the function almost simultaneously with function execution.
			This function internally creates a thread to train, so you do not need to write an explicit thread wrapper when you want to do something else.
			When users use this function, DetectionTrainer will use a certain percentage of the training set as a validation set.
			* @param trainData DetectionTrainData to train. See DetectionTrainData for details.
			* @return Returns Status.
			*/
			Status __stdcall StartTrain(const DetectionTrainData& trainData);
			/**
			* @brief Trainer starts to train artificial neural network by calling this function.
			* @details This function takes just two arguments. They are both DetectionTrainData. One is for train data, and the other is for validation data.
			It works asynchronously and terminates the function almost simultaneously with function execution.
			This function internally creates a thread to train, so you do not need to write an explicit thread wrapper when you want to do something else.
			This function explicitly sets the validation set and proceeds with the learning.
			* @param trainData DetectionTrainData to train. See DetectionTrainData for details.
			* @param validationData Learn by specifying explicit validation data.
			* @return Returns Status.
			*/
			Status __stdcall StartTrain(const DetectionTrainData& trainData, const DetectionTrainData& validationData);

			/**
			* @brief Blocking function until the train is finished.
			* @details Calling this function will block until the train is terminated.
			* @return Returns Status.
			*/
			Status __stdcall WaitTrain();

			/**
			* @brief Returns a Message representing the current training state.
			* @details DetectionTrainer pushes its message into an internally managed message queue and proceeds with learning as it progresses.
			When the GetMessage function is called, the internally managed message in the queue is dequeued and returned to the user as a Message object.
			If there is no message in the internal message queue, it will be blocked until a message is enqueued. If a message is enqueued, the message will be dequeued and returned immediately.
			* @return Returns Message object.
			*/
			Message __stdcall GetMessage();

			/**
			* @brief Returns internal Message queue is empty
			* @details DetectionTrainer has message queue for communication. This function returns true when that queue is empty, otherwise return false.
			* @return is message queue is empty
			*/
			bool __stdcall MessageIsEmpty();

			/**
			* @brief If training is currently in progress, we stop training. If the training is not in progress, nothing happens on the call.
			* @return Returns Status.
			*/
			Status __stdcall StopTrain();

			/**
			* @brief Explicit resource release. It is useful in non-native languages (such as C#).
			* @return Returns Status.
			*/
			Status __stdcall Destroy();
		private:
			DetectionTrainer(const DetectionTrainer& src); //=delete;
			DetectionTrainer& operator=(const DetectionTrainer& rhs); //=delete;
			DeviceDescriptor m_devDesc;
			InputDataType m_dataType;
			Status m_status;
			void* internal;
		};

		/** 
		* @brief Structure containing model information of trained model. 
		* @details Each parameters are the same as TrainConfig parameters.
		* @author ?Sualab SuaKIT Team
		*/
		struct SUAKIT_API TrainedModelInfo
		{
			AugmentationConfig augmentationConfig;
			Int32Array dataRatio;
			FloatArray epochForLossGraph, lossForLossGraph, anchors;
			float minimumEpoch, savedModelEpoch, minLoss, endEpoch;
			SuaKIT_UInt64 multiImgCount, numberOfClasses;
			SuaKIT_UInt64 imageHeight, imageWidth, imageChannel;
			SuaKIT_UInt64 validationRatio, epochs, modelCapacity;
			SuaKIT_UInt64 numModelSelection;
			SuaKIT_UInt64 patchSize, strideRatio, unlabeledRatio;
			SuaKIT_UInt64 numTrainImage;
			SuaKIT_Int64 patience;
			static const uchar BUFF_SIZE = 32;
			char trainElapsedTime[BUFF_SIZE], modelSavedTime[BUFF_SIZE];
			ModelType modelType;
			InputDataType inputDataType;
			DepthType imageDepth;
		};

		/** 
		* @brief Get the information of trained model.
		* @param trainedModelInfo Information of trained model.
		* @param modelPath The trained model path for executing GetTrainedModelInfo.
		* @return Returns Status.
		* @author ?Sualab SuaKIT Team
		*/
		SUAKIT_API Status __stdcall GetTrainedModelInfo(TrainedModelInfo& trainedModelInfo, const wchar_t* modelPath);

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
		* @author ?Sualab SuaKIT Team
		*/
		SUAKIT_API Status __stdcall isContinueTrainableBaseModel(ModelType modelType, SuaKIT_Int32 imgCnt, SuaKIT_Int32 numberOfClasses, const wchar_t *modelPath);
	}
}