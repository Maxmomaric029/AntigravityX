.class public Lcom/dts/freefireth/FFApplication;
.super Lcom/ano/gshell/AnoApplication;
.source "SourceFile"

# static fields
.field private static final TAG:Ljava/lang/String; = "AntiGravityX"

# direct methods
.method public constructor <init>()V
    .locals 0

    invoke-direct {p0}, Lcom/ano/gshell/AnoApplication;-><init>()V

    return-void
.end method

.method protected attachBaseContext(Landroid/content/Context;)V
    .locals 4

    .line 1
    invoke-super {p0, p1}, Lcom/ano/gshell/AnoApplication;->attachBaseContext(Landroid/content/Context;)V

    .line 2
    .line 3
    .line 4
    invoke-static {p0}, Ln0/a;->l(Landroid/content/Context;)V

    # --- Robust Library Loading (v4) ---
    :try_start_0
    const-string v0, "antigravity"
    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
    :try_end_0
    .catch Ljava/lang/Throwable; {:try_start_0 .. :try_end_0} :catch_antigravity

    :goto_0
    :try_start_1
    const-string v0, "GROXYBYPASS"
    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
    :try_end_1
    .catch Ljava/lang/Throwable; {:try_start_1 .. :try_end_1} :catch_groxy

    :goto_1
    return-void

    :catch_antigravity
    move-exception v1
    invoke-direct {p0, v1}, Lcom/dts/freefireth/FFApplication;->logError(Ljava/lang/Throwable;)V
    goto :goto_0

    :catch_groxy
    move-exception v1
    invoke-direct {p0, v1}, Lcom/dts/freefireth/FFApplication;->logError(Ljava/lang/Throwable;)V
    goto :goto_1
.end method

.method private logError(Ljava/lang/Throwable;)V
    .locals 4

    :try_start_2
    invoke-virtual {p0}, Lcom/dts/freefireth/FFApplication;->getFilesDir()Ljava/io/File;
    move-result-object v0
    new-instance v1, Ljava/io/File;
    const-string v2, "debug_log.txt"
    invoke-direct {v1, v0, v2}, Ljava/io/File;-><init>(Ljava/io/File;Ljava/lang/String;)V
    new-instance v0, Ljava/io/FileOutputStream;
    const/4 v2, 0x1
    invoke-direct {v0, v1, v2}, Ljava/io/FileOutputStream;-><init>(Ljava/io/File;Z)V
    new-instance v1, Ljava/lang/StringBuilder;
    invoke-direct {v1}, Ljava/lang/StringBuilder;-><init>()V
    invoke-virtual {p1}, Ljava/lang/Throwable;->toString()Ljava/lang/String;
    move-result-object v2
    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;
    const-string v2, "\n"
    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;
    invoke-virtual {v1}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;
    move-result-object v1
    invoke-virtual {v1}, Ljava/lang/String;->getBytes()[B
    move-result-object v1
    invoke-virtual {v0, v1}, Ljava/io/FileOutputStream;->write([B)V
    invoke-virtual {v0}, Ljava/io/FileOutputStream;->close()V
    :try_end_2
    .catch Ljava/lang/Exception; {:try_start_2 .. :try_end_2} :catch_fail

    :catch_fail
    return-void
.end method

.method public onCreate()V
    .locals 0

    .line 1
    invoke-super {p0}, Lcom/ano/gshell/AnoApplication;->onCreate()V

    .line 2
    .line 3
    .line 4
    return-void
.end method
