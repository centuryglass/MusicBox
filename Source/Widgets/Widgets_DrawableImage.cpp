#include "Widgets_DrawableImage.h"
#include "Assets.h"
#include <map>

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Widgets::DrawableImage::";
#endif


// Creates a DrawableImage using an image file path.
Widgets::DrawableImage::DrawableImage
(const juce::String assetFilename, const juce::RectanglePlacement placement) :
DrawableImage(placement)
{
    setImage(assetFilename);
}


// Creates a DrawableImage using an image file.
Widgets::DrawableImage::DrawableImage
(const juce::File imageFile, const juce::RectanglePlacement placement) :
DrawableImage(placement)
{
    setImage(imageFile);
}


// Creates a DrawableImage using an image object.
Widgets::DrawableImage::DrawableImage
(const juce::Image image, const juce::RectanglePlacement placement) :
DrawableImage(placement)
{
    setImage(image);
}


// Creates DrawableImage using a Drawable object.
Widgets::DrawableImage::DrawableImage
(juce::Drawable* drawable, const juce::RectanglePlacement placement) :
DrawableImage(placement)
{
    setImage(drawable);
}


// Creates a DrawableImage without an initial image.
Widgets::DrawableImage::DrawableImage
(const juce::RectanglePlacement placement) :
placement(placement)
{
    #if JUCE_DEBUG
    setName("Widgets::DrawableImage");
    #endif
    setInterceptsMouseClicks(false, false);
}


// Changes the image drawn by this component.
void Widgets::DrawableImage::setImage(const juce::String assetFilename)
{
    setImage(Assets::findAssetFile(assetFilename));
}


// Changes the image drawn by this component.
void Widgets::DrawableImage::setImage(const juce::File imageFile)
{
    const juce::MessageManagerLock mmLock;
    if (imageFile.existsAsFile())
    {
        if (imageDrawable != nullptr)
        {
            removeChildComponent(imageDrawable.get());
        }
        imageDrawable.reset(juce::Drawable::createFromImageFile(imageFile));
        if (imageDrawable != nullptr)
        {
            imageSource = imageFile;
            initImage();
        }
        else
        {
            DBG(dbgPrefix << __func__ << ": Failed to load "
                    << imageFile.getFullPathName());
        }
    }
    else
    {
        imageDrawable.reset(nullptr);
    }
}


// Changes the image drawn by this component.
void Widgets::DrawableImage::setImage(const juce::Image image)
{
    juce::DrawableImage * drawable = new juce::DrawableImage();
    drawable->setImage(image);
    imageDrawable.reset(drawable);
    initImage();
}


// Changes the image drawn by this component.
void Widgets::DrawableImage::setImage(juce::Drawable* drawable)
{
    imageDrawable.reset(drawable);
    initImage();
}


// Checks if an image is set for this component.
bool Widgets::DrawableImage::hasImage()
{
    return imageDrawable != nullptr && !imageDrawable->getBounds().isEmpty();
}


// Checks if an image is set for this component.
bool Widgets::DrawableImage::isEmpty()
{
    return !hasImage();
}


// Adjusts image size and placement whenever component size changes.
void Widgets::DrawableImage::resized()
{
    if (imageDrawable != nullptr)
    {
        imageDrawable->setTransformToFit(getLocalBounds().toFloat(),
                placement);
    }
}


// Sets the initial image colours and scale.
void Widgets::DrawableImage::initImage()
{
    if (imageDrawable == nullptr)
    {
        return;
    }
    addAndMakeVisible(imageDrawable.get());
    imageDrawable->setTransformToFit(getLocalBounds().toFloat(), placement);
}
