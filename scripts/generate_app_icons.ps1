Add-Type -AssemblyName System.Drawing

function Draw-Icon {
    param(
        [int]$size,
        [string]$outPath,
        [bool]$isRound = $false
    )

    $bmp = New-Object System.Drawing.Bitmap $size, $size
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
    $g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
    $g.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
    $g.Clear([System.Drawing.Color]::Transparent)

    $rect = New-Object System.Drawing.RectangleF 0, 0, $size, $size
    $path = New-Object System.Drawing.Drawing2D.GraphicsPath

    if ($isRound) {
        $path.AddEllipse(2, 2, ($size - 4), ($size - 4))
    } else {
        # Rounded squircle
        $corner = $size * 0.22
        $d = $corner * 2
        $path.AddArc(2, 2, $d, $d, 180, 90)
        $path.AddArc(($size - $d - 2), 2, $d, $d, 270, 90)
        $path.AddArc(($size - $d - 2), ($size - $d - 2), $d, $d, 0, 90)
        $path.AddArc(2, ($size - $d - 2), $d, $d, 90, 90)
        $path.CloseFigure()
    }

    # Base Obsidian Background Brush
    $bgBrush = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(255, 8, 9, 12))
    $g.FillPath($bgBrush, $path)
    $bgBrush.Dispose()

    # Inner subtle radial amber glow
    $glowPath = New-Object System.Drawing.Drawing2D.GraphicsPath
    $glowPath.AddEllipse(($size * 0.15), ($size * 0.15), ($size * 0.7), ($size * 0.7))
    $pgh = New-Object System.Drawing.Drawing2D.PathGradientBrush $glowPath
    $pgh.CenterColor = [System.Drawing.Color]::FromArgb(60, 245, 158, 11)
    $pgh.SurroundColors = @([System.Drawing.Color]::FromArgb(0, 8, 9, 12))
    $g.FillPath($pgh, $glowPath)
    $pgh.Dispose()
    $glowPath.Dispose()

    # Gold Prism Border
    $pen = New-Object System.Drawing.Pen ([System.Drawing.Color]::FromArgb(180, 251, 191, 36), [Math]::Max(1.5, $size * 0.03))
    $g.DrawPath($pen, $path)
    $pen.Dispose()

    # Draw Relay Lightning / Power Emblem (Golden Amber)
    $boltPath = New-Object System.Drawing.Drawing2D.GraphicsPath
    # Polygon coordinates scaled to size
    $p1 = New-Object System.Drawing.PointF ($size * 0.54), ($size * 0.18)
    $p2 = New-Object System.Drawing.PointF ($size * 0.32), ($size * 0.52)
    $p3 = New-Object System.Drawing.PointF ($size * 0.48), ($size * 0.52)
    $p4 = New-Object System.Drawing.PointF ($size * 0.42), ($size * 0.82)
    $p5 = New-Object System.Drawing.PointF ($size * 0.70), ($size * 0.44)
    $p6 = New-Object System.Drawing.PointF ($size * 0.54), ($size * 0.44)
    $points = [System.Drawing.PointF[]]@($p1, $p2, $p3, $p4, $p5, $p6)
    $boltPath.AddPolygon($points)

    # Gradient fill for Bolt (Bright Yellow #FDE047 to Rich Gold #F59E0B)
    $boltBrush = New-Object System.Drawing.Drawing2D.LinearGradientBrush(
        (New-Object System.Drawing.PointF ($size * 0.3), ($size * 0.2)),
        (New-Object System.Drawing.PointF ($size * 0.7), ($size * 0.8)),
        [System.Drawing.Color]::FromArgb(255, 253, 224, 71),
        [System.Drawing.Color]::FromArgb(255, 217, 119, 6)
    )
    $g.FillPath($boltBrush, $boltPath)
    $boltBrush.Dispose()

    # Specular bolt rim outline
    $boltPen = New-Object System.Drawing.Pen ([System.Drawing.Color]::FromArgb(240, 255, 255, 255), [Math]::Max(1.0, $size * 0.018))
    $g.DrawPath($boltPen, $boltPath)
    $boltPen.Dispose()
    $boltPath.Dispose()

    # Two Small Relay Contact Nodes at the bottom
    $nodeBrush = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(220, 251, 191, 36))
    $nodeR = $size * 0.04
    $g.FillEllipse($nodeBrush, ($size * 0.24), ($size * 0.74), ($nodeR * 2), ($nodeR * 2))
    $g.FillEllipse($nodeBrush, ($size * 0.68), ($size * 0.74), ($nodeR * 2), ($nodeR * 2))
    $nodeBrush.Dispose()

    # Clean up and save
    $path.Dispose()
    $g.Dispose()

    $dir = [System.IO.Path]::GetDirectoryName($outPath)
    if (!(Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null
    }
    $bmp.Save($outPath, [System.Drawing.Imaging.ImageFormat]::Png)
    $bmp.Dispose()
    Write-Output "Created: $outPath ($size x $size)"
}

$densities = @(
    @{ name = "mipmap-mdpi"; size = 48 },
    @{ name = "mipmap-hdpi"; size = 72 },
    @{ name = "mipmap-xhdpi"; size = 96 },
    @{ name = "mipmap-xxhdpi"; size = 144 },
    @{ name = "mipmap-xxxhdpi"; size = 192 }
)

$baseRes = "android_app\app\src\main\res"

foreach ($d in $densities) {
    $folder = Join-Path $baseRes $d.name
    Draw-Icon -size $d.size -outPath (Join-Path $folder "ic_launcher.png") -isRound $false
    Draw-Icon -size $d.size -outPath (Join-Path $folder "ic_launcher_round.png") -isRound $true
}

# Also save to drawable for fallback
Draw-Icon -size 192 -outPath (Join-Path $baseRes "drawable\ic_launcher.png") -isRound $false

Write-Output "App Icons generation complete!"

