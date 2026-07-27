import sharp from 'sharp';
import pngToIco from 'png-to-ico';
import fs from 'fs';
import path from 'path';

async function generateIcons() {
  const svgPath = path.resolve('res/app_icon.svg');

  if (!fs.existsSync(svgPath)) {
    console.error('app_icon.svg not found!');
    return;
  }

  console.log('Rendering PNG icons from SVG...');

  const sizes = [16, 32, 48, 64, 128, 256];
  const pngPaths = [];

  const tempDir = path.resolve('temp_icons');
  if (!fs.existsSync(tempDir)) fs.mkdirSync(tempDir, { recursive: true });

  for (const size of sizes) {
    const pngPath = path.join(tempDir, `icon_${size}.png`);
    await sharp(svgPath)
      .resize(size, size)
      .png()
      .toFile(pngPath);
    pngPaths.push(pngPath);
  }

  // Generate 256x256 high-res PNG for website favicon & app branding
  const publicDir = path.resolve('website/public');
  if (!fs.existsSync(publicDir)) fs.mkdirSync(publicDir, { recursive: true });

  const mainPng = path.join(publicDir, 'favicon.png');
  await sharp(svgPath).resize(512, 512).png().toFile(mainPng);
  await sharp(svgPath).resize(512, 512).png().toFile(path.resolve('res/app_icon.png'));

  console.log('Generating multi-resolution .ico files...');
  const icoBuffer = await pngToIco(pngPaths);

  fs.writeFileSync(path.join(publicDir, 'favicon.ico'), icoBuffer);
  fs.writeFileSync(path.join(publicDir, 'app.ico'), icoBuffer);
  fs.writeFileSync(path.resolve('res/app.ico'), icoBuffer);
  fs.writeFileSync(path.resolve('src/app.ico'), icoBuffer);

  // Clean up temp PNGs
  fs.rmSync(tempDir, { recursive: true, force: true });

  console.log('✅ ALL ICONS GENERATED SUCCESSFULLY!');
  console.log('- website/public/favicon.ico');
  console.log('- website/public/favicon.png');
  console.log('- res/app.ico');
  console.log('- src/app.ico');
}

generateIcons().catch(console.error);
